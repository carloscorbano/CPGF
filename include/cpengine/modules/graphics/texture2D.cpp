#include "texture2D.hpp"
#include "../threading/multithread.hpp"
#include "../resources/filesystem.hpp"
#include "../resources/resources.hpp"
#include "../../engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace CPGFramework 
{
    namespace Graphics 
    {
        Texture2D::Texture2D(Engine* engine, const STRING& relativePath, const STRING& filename) 
            : ResourceBase(engine, relativePath, filename), m_ID(INVALID_ID) 
        {
            DEBUG_LOG("Creating texture2D, starting the creation pipeline, Texture2D name:", filename);
            __INTERNAL__ProcessPipelineCreation();
        }

        Texture2D::~Texture2D() 
        {
            DEBUG_LOG("Destroying texture 2D, name:", GetFilename());
            if (!alreadyCompiled)
            {
                DEBUG_ERROR("The ID of the Texture2D submitted was invalid!");
            }
            else
            {
                DEBUG_SUCCESS("Successfully destroyed texture2D!");
                glDeleteTextures(1, &m_ID);
                alreadyCompiled = false;
            }
        }

        void Texture2D::HotReload() 
        {
            DEBUG_LOG("Texture 2D Hot Reload requested for file:", GetFilename());
            __INTERNAL__ProcessPipelineCreation();
        }

        void Texture2D::Bind(GLenum textureUnit) 
        {
            if(GetResourceState() == Resources::ResourceState::OK) 
            {
                glActiveTexture(textureUnit);
		        glBindTexture(GL_TEXTURE_2D, m_ID);
            }
        }

        ui32 Texture2D::GetInternalFormat() const 
        {
            return m_internalFormat;
        }

        ui32 Texture2D::GetFormat() const 
        {
            return m_format;
        }

        ui32 Texture2D::GetID() const 
        {
            return m_ID;
        }

        void Texture2D::__INTERNAL__ProcessPipelineCreation() 
        {
            DEBUG_LOG("Starting the Texture2D pipeline creation...");

            struct tex_data : Threading::OperationData 
            {
                Engine* engine;
                std::vector<char> buffer;
                Texture2D* self;

                tex_data() 
                {
                    self = nullptr;
                }

                ~tex_data() override 
                {
                    DEBUG_LOG("Deleting pipeline tex data.");
                }
            };

            SetResourceState(Resources::ResourceState::LOADING);

            EngineModules* modules = engine->GetModules();
            Threading::WorkDescriptor wd(modules->multithread.get());
            tex_data* td = wd.BindData<tex_data>();
            td->self = this;
            td->engine = engine;

            std::thread::id gameLoopID = engine->GetGameLoopID();
            std::thread::id resourcesLoopID = engine->GetResourcesLoopID();

            wd.BindOperation(NULL_THREAD_ID, [](Threading::OperationData* data) 
            {
                tex_data* td = reinterpret_cast<tex_data*>(data);
                Texture2D* self = td->self;
                Resources::FileSystem& fs = td->engine->GetModules()->resources->GetFileSystem();

                td->buffer = fs.ReadFileBuffer(self->GetRelativePath(), self->GetFilename());

                if(td->buffer.empty()) 
                {
                    DEBUG_ERROR("Failed to read buffer from the texture file, name:", self->GetFilename());
                    self->SetResourceState(Resources::ResourceState::ERROR);
                    return false;
                }

                return true;
            });

            wd.BindOperation(resourcesLoopID, [](Threading::OperationData* data) 
            {
                tex_data* td = reinterpret_cast<tex_data*>(data);
                Texture2D* self = td->self;

                ui32 ID = INVALID_ID;

                glGenTextures(1, &ID);
                DEBUG_LOG("Generating texture ID:", ID);

                glBindTexture(GL_TEXTURE_2D, ID);

                self->SetupTexParameters();
                stbi_set_flip_vertically_on_load(true);
                
                const char* buf = &td->buffer[0];

                i32 width = 0, height = 0, nrChannels = 0;
				stbi_uc* image_data = stbi_load_from_memory(
                     (const stbi_uc*)buf,
                     td->buffer.size(),
                     &width, &height, &nrChannels, 0);

                self->__INTERNAL__Setup(width, height, nrChannels);
                self->__INTERNAL__SetupInternalFormat(nrChannels);
                
                if(image_data) 
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, self->GetInternalFormat(), width, height, 0, self->GetFormat(), GL_UNSIGNED_BYTE, image_data);
					glGenerateMipmap(GL_TEXTURE_2D);
                } 
                else 
                {
                    DEBUG_ERROR("Invalid Image Data! Failed to compile texture name:", self->GetFilename());
                    glDeleteTextures(1, &ID);
                    self->SetResourceState(Resources::ResourceState::ERROR);
                    return false;
                }

                stbi_image_free(image_data);

                if(self->alreadyCompiled) 
                {
                    DEBUG_SUCCESS("Successfully hot reload from texture", self->GetFilename());
                    glDeleteTextures(1, &self->m_ID);
                }

                DEBUG_SUCCESS("Successfuly compiled texture", self->GetFilename());
                self->m_ID = ID;

                glFinish();
                self->SetResourceState(Resources::ResourceState::OK);
                self->alreadyCompiled = true;
                return true;
            });

            wd.Submit();
        }

        void Texture2D::SetupTexParameters(ui32 wrapping, ui32 filterMin, ui32 filterMag, BOOL bind) 
        {
            if(bind) Bind();

            m_wrappingMode = wrapping;
            m_filterMin = filterMin;
            m_filterMag = filterMag;

            //texture wrapping
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
		    //filter
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
        }

        void Texture2D::AssignTextureData(i32 width, i32 height, i32 nChannels, const void* pixels, GLenum type, BOOL bind) 
        {
            if(bind) Bind();

            m_width = width;
            m_height = height;

            __INTERNAL__SetupInternalFormat(nChannels);

            glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, m_format, type, pixels);
		    glGenerateMipmap(GL_TEXTURE_2D);
        }

        void Texture2D::AssignSubTextureData(i32 xoffset, i32 yoffset, i32 width, i32 height, const void* pixels, GLenum type, BOOL bind) 
        {
            if (    xoffset < 0 || xoffset >= m_width ||
			        yoffset < 0 || yoffset >= m_height ||
			        xoffset + width >= m_width ||
			        yoffset + height >= m_height ||
			        pixels == nullptr) 
            {
                return;
            }

            if(bind) Bind();

		    glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, m_format, type, pixels);
		    glGenerateMipmap(GL_TEXTURE_2D);
        }

        void Texture2D::__INTERNAL__Setup(i32 width, i32 height, i32 nrChannels) 
        {
            m_width = width;
            m_height = height;
            m_nrChannels = nrChannels;
        }

        void Texture2D::__INTERNAL__SetupInternalFormat(i32 numberOfChannels) 
        {
            switch (numberOfChannels)
            {
                case 1:
                {
                    m_internalFormat = GL_R8;
                    m_format = GL_RED;
                }   break;
                case 2:
                {
                    m_internalFormat = GL_RG8;
                    m_format = GL_RG;
                }   break;
                case 3:
                {
                    m_internalFormat = GL_RGB8;
                    m_format = GL_RGB;
                }   break;
                case 4:
                {
                    m_internalFormat = GL_RGBA8;
                    m_format = GL_RGBA;
                }   break;
                default: break;
            }
        }

        void Texture2D::__INTERNAL__SetupInternalFormat(ui32 internalFormat, ui32 format) 
        {
            m_internalFormat = internalFormat;
            m_format = format;
        }      
    }
}