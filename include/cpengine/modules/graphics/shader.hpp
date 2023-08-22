#pragma once

#include "../resources/resource_base.hpp"
#include "../../containers/unordered_ref_type_map.hpp"
#include "glfw.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <map>

namespace CPGFramework 
{
    namespace Graphics 
    {
        class Shader : public Resources::ResourceBase 
        {
        public:
            Shader(Engine* engine, const STRING& relativePath, const STRING& filename);
            ~Shader() override;

            void HotReload() override;

            void Bind() const;
            const ui32 GetID() const;
            void SetID(const ui32& newID);

            template<typename T>
            void SetUniform(const STRING& name, const T& value, const BOOL& useShader = false)
            {
                ASSERT(false);
            }

            template<>
            void SetUniform<FLOAT>(const STRING& name, const FLOAT& value, const BOOL& useShader)
            {
                if(useShader) Bind();
                GLCall(glUniform1f(GetMappedUniformLocation<FLOAT>(name), value));
            }

            template<>
            void SetUniform<i32>(const STRING& name, const i32& value, const BOOL& useShader)
            {
                if(useShader) Bind();
                GLCall(glUniform1i(GetMappedUniformLocation<i32>(name), value));
            }

            template<>
            void SetUniform<VEC2>(const STRING& name, const VEC2& value, const BOOL& useShader)
            {
                if(useShader) Bind();
                GLCall(glUniform2f(GetMappedUniformLocation<VEC2>(name), value.x, value.y));
            }
            
            template<>
            void SetUniform<VEC3>(const STRING& name, const VEC3& value, const BOOL& useShader)
            {
                if(useShader) Bind();
                GLCall(glUniform3f(GetMappedUniformLocation<VEC3>(name), value.x, value.y, value.z));
            }
            
            template<>
            void SetUniform<VEC4>(const STRING& name, const VEC4& value, const BOOL& useShader)
            {
                if(useShader) Bind();
                GLCall(glUniform4f(GetMappedUniformLocation<VEC4>(name), value.x, value.y, value.z, value.w));
            }

            template<>
            void SetUniform<MAT4>(const STRING& name, const MAT4& value, const BOOL& useShader)
            {
                if(useShader) Bind();
                GLCall(glUniformMatrix4fv(GetMappedUniformLocation<MAT4>(name), 1, false, glm::value_ptr(value)));
            }
            
            void SetUniformBlockIndex(const STRING& name, const i32& value, const BOOL& useShader = false);

            template<typename T>
            i32 GetMappedUniformLocation(const STRING& name) 
            {
                std::map<STRING, i32>& map = m_uniforms[typeid(T)];
                std::map<STRING, i32>::iterator it = map.find(name);
                if (it != map.end()) 
                {
                    return map[name];
                }

                map[name] = glGetUniformLocation(this->m_ID, name.c_str());
                return map[name];
            }

            static BOOL CheckCompileErrors(const GLuint& shader, const STRING& type);
        private:
            void __INTERNAL__ProcessPipelineCreation();
        private:
            Containers::UnorderedRefTypeMap<std::map<STRING, i32>> m_uniforms;
            ui32 m_ID;
        };
    }
}