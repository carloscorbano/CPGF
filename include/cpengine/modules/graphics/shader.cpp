#include "shader.hpp"
#include "../threading/multithread.hpp"
#include "../resources/filesystem.hpp"
#include "../resources/resources.hpp"
#include "../../engine.hpp"

namespace CPGFramework 
{
    namespace Graphics 
    {
        Shader::Shader(Engine* engine, const STRING& relativePath, const STRING& filename)
            : Resources::ResourceBase(engine, relativePath, filename), m_ID(INVALID_ID) 
        {
            DEBUG_LOG("Creating shader... Starting the creation pipeline... Shader name:", filename);
            __INTERNAL__ProcessPipelineCreation();
        }
        
        Shader::~Shader() 
        {
            DEBUG_LOG("Destroying shader resource. Name:", GetFilename());
            if(!alreadyCompiled) 
            {
                DEBUG_WARNING("Shader that was not compiled received a deletion callback.");
            } 
            else 
            {
                DEBUG_LOG("Deleting shader with ID", m_ID);
                GLCall(glDeleteProgram(m_ID));
                alreadyCompiled = false;
            }
        }

        void Shader::HotReload() 
        {
            DEBUG_LOG("Shader Hot Reload requested... Shader name:", GetFilename());
            __INTERNAL__ProcessPipelineCreation();
        }

        void Shader::Bind() const
        {
            if(GetResourceState() == Resources::ResourceState::OK) 
            {
                GLCall(glUseProgram(this->m_ID));
            }
        }

        const ui32 Shader::GetID() const 
        {
            return m_ID;
        }

        void Shader::SetID(const ui32& newID) 
        {
            m_ID = newID;
        }

        void Shader::__INTERNAL__ProcessPipelineCreation() 
        {
            struct shader_comp_data : public Threading::OperationData 
            {
                shader_comp_data() 
                {
                    self = nullptr;
                }

                ~shader_comp_data() override 
                {
                    DEBUG_LOG("Destroyed shader compilation data...");
                }

                Engine* engine;
                Shader* self;
                std::vector<char> vertBuffer;
                std::vector<char> fragBuffer;
                std::vector<char> geoBuffer;
            };

            const std::thread::id mainID = engine->GetGameLoopID();
            const std::thread::id resID = engine->GetResourcesLoopID();

            Threading::WorkDescriptor wd(engine->GetModules()->multithread.get());
            shader_comp_data* boundedData = wd.BindData<shader_comp_data>();
            boundedData->self = this;
            boundedData->engine = engine;
            SetResourceState(Resources::ResourceState::LOADING);

            //Load operation
            wd.BindOperation(NULL_THREAD_ID, [](Threading::OperationData* data) 
            {
                shader_comp_data* casted = reinterpret_cast<shader_comp_data*>(data);
                EngineModules* modules = casted->engine->GetModules();
                Resources::FileSystem& fs = modules->resources->GetFileSystem();
                Shader* self = casted->self;
                
                casted->vertBuffer = fs.ReadFileBuffer(self->GetRelativePath(), self->GetFilename().append(".vs"));
                casted->fragBuffer = fs.ReadFileBuffer(self->GetRelativePath(), self->GetFilename().append(".fs"));

                try 
                {
                    casted->geoBuffer = fs.ReadFileBuffer(self->GetRelativePath(), self->GetFilename().append(".gs"));
                } 
                catch(const std::exception& ex) 
                {
                    DEBUG_WARNING("Geometry shader not found... continuing to load shader with vert and frag code.");
                }

                if(casted->vertBuffer.empty()) 
                {
                    DEBUG_ERROR("Failed to get the vertex file buffer...");
                    return false;
                }

                if(casted->fragBuffer.empty()) 
                {
                    DEBUG_ERROR("Failed to get the fragment buffer file data...");
                    return false;
                }

                return true;
            });
            //Compile operation
            wd.BindOperation(resID, [](Threading::OperationData* data)
            {
                shader_comp_data* casted = reinterpret_cast<shader_comp_data*>(data);
                Shader* self = casted->self;

                const char* vert = &casted->vertBuffer[0];
                const char* frag = &casted->fragBuffer[0];

                const GLint vsize = casted->vertBuffer.size();
                const GLint fsize = casted->fragBuffer.size();

                ui32 v, f, g;
                GLCall(v = glCreateShader(GL_VERTEX_SHADER));
                GLCall(glShaderSource(v, 1,
                    &vert,
                    (GLint*)&vsize));
                GLCall(glCompileShader(v));
                if(!Shader::CheckCompileErrors(v, "VERTEX")) 
                {
                    GLCall(glDeleteShader(v));
                    DEBUG_ERROR("Failed to compile vertex shader");
                    self->SetResourceState(Resources::ResourceState::ERROR);
                    return false;
                }

                GLCall(f = glCreateShader(GL_FRAGMENT_SHADER));
                GLCall(glShaderSource(f, 1,
                    &frag, 
                    (GLint*)&fsize));
                GLCall(glCompileShader(f));
                if(!Shader::CheckCompileErrors(f, "FRAGMENT")) 
                {
                    GLCall(glDeleteShader(v));
                    GLCall(glDeleteShader(f));
                    DEBUG_ERROR("Failed to compile fragment shader");
                    self->SetResourceState(Resources::ResourceState::ERROR);
                    return false;
                }

                if(casted->geoBuffer.size() > 0) 
                {
                    const char* geo = &casted->geoBuffer[0];
                    const GLint gsize = casted->geoBuffer.size();

                    GLCall(g = glCreateShader(GL_GEOMETRY_SHADER));
                    GLCall(glShaderSource(g, 1,
                        &geo, 
                        (GLint*)&gsize));
                    GLCall(glCompileShader(g));
                    if(!Shader::CheckCompileErrors(g, "GEOMETRY")) 
                    {
                        GLCall(glDeleteShader(v));
                        GLCall(glDeleteShader(f));
                        GLCall(glDeleteShader(g));
                        DEBUG_ERROR("Failed to compile geometry shader");
                        self->SetResourceState(Resources::ResourceState::ERROR);
                        return false;
                    }
                }

                GLCall(ui32 ID = glCreateProgram());
                DEBUG_LOG("Compiling shader ID:", ID);
                GLCall(glAttachShader(ID, v));
                GLCall(glAttachShader(ID, f));
                if(casted->geoBuffer.size() > 0) glAttachShader(ID, g);

                GLCall(glLinkProgram(ID));
                if(!Shader::CheckCompileErrors(ID, "PROGRAM")) 
                {
                    GLCall(glDeleteShader(v));
                    GLCall(glDeleteShader(f));
                    if(casted->geoBuffer.size() > 0) glDeleteShader(g);
                    self->SetResourceState(Resources::ResourceState::ERROR);
                    DEBUG_ERROR("Failed to compile shader program...");
                    return false;
                }

                GLCall(glDeleteShader(v));
                GLCall(glDeleteShader(f));
                if(casted->geoBuffer.size() > 0) glDeleteShader(g);

                const i32 selfCurID = (i32)self->GetID();
                if(self->alreadyCompiled) 
                {
                    DEBUG_LOG("Deleting old compiled shader ID:", selfCurID, "- New Shader ID:", ID);
                    GLCall(glDeleteProgram(selfCurID));
                }

                self->SetID(ID);
                GLCall(glFinish());
                DEBUG_SUCCESS("Finished compiling shader ID:", ID);
                self->SetResourceState(Resources::ResourceState::OK);
                self->alreadyCompiled = true;
                
                return true;
            });

            wd.BindCallback(mainID, [](Threading::OperationData* data, const BOOL& success)
            {
                shader_comp_data* casted = reinterpret_cast<shader_comp_data*>(data);
                Shader* self = casted->self;

                if(!success) 
                {
                    return;
                }

                self->Bind();
                //self->SetUniformBlockIndex("CameraBlock", 0);
            });

            DEBUG_LOG("Shader pipeline submitted...");
            wd.Submit();
        }

        void Shader::SetUniformBlockIndex(const STRING& name, const i32& value, const BOOL& useShader) 
        {
            if (useShader) this->Bind();
            GLCall(ui32 uniformIndex = glGetUniformBlockIndex(this->m_ID, name.c_str()));
            GLCall(glUniformBlockBinding(this->m_ID, uniformIndex, value));
        }

        BOOL Shader::CheckCompileErrors(const GLuint& shader, const STRING& type) 
        {
            i32 success;
            char infoDEBUG[1024];
            if(type != "PROGRAM") 
            {
                GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
                if (!success) 
                {
                    GLCall(glGetShaderInfoLog(shader, 1024, NULL, infoDEBUG));
                    DEBUG_ERROR("::SHADER_INTERNAL::ERROR::SHADER_COMPILATION_ERROR of type: ", type);
                    DEBUG_ERROR("-----------------------[ERROR DEBUG]-------------------------");
                    DEBUG_ERROR(infoDEBUG);
                    DEBUG_ERROR("-----------------------------------------------------------");
                    return false;
                }
            } 
            else 
            {
                GLCall(glGetProgramiv(shader, GL_LINK_STATUS, &success));
                if (!success)
                {
                    GLCall(glGetProgramInfoLog(shader, 1024, NULL, infoDEBUG));
                    DEBUG_ERROR("::SHADER_INTERNAL::ERROR::PROGRAM_LINKING_ERROR of type: ", type);
                    DEBUG_ERROR("-----------------------[ERROR DEBUG]-------------------------");
                    DEBUG_ERROR(infoDEBUG);
                    DEBUG_ERROR("-----------------------------------------------------------");
                    return false;
                }
            }

            return true;
        }
    }
}