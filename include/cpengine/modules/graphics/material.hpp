#pragma once

#include "texture2D.hpp"
#include "shader.hpp"
#include <any>

namespace CPGFramework
{
    namespace Graphics
    {
        enum class AttributeType
        {
            FLOAT, I32, VEC2, VEC3, VEC4, MAT4, TEX2D
        };

        struct TextureData
        {
            Texture2D* texture;
            GLenum unit;
            TextureData() : texture(nullptr), unit(GL_TEXTURE0) {}
            TextureData(Texture2D* texture, GLenum unit) : texture(texture), unit(unit) {}
        };

        struct MaterialAttribute
        {
            STRING name;
            AttributeType type;
            std::any value;

            MaterialAttribute() : name(""), type(AttributeType::I32) {}
            MaterialAttribute(STRING name, AttributeType type, std::any value) : name(name), type(type), value(value) {}
        };

        class Material
        {
        public:
            Material();
            ~Material();

            template<typename T>
            void Push(const STRING& name, const T& value)
            {
                ASSERT(false);
            }

            template<>
            void Push<FLOAT>(const STRING& name, const FLOAT& value)
            {
                m_attributes.push_back({ name, AttributeType::FLOAT, value });
            }

            template<>
            void Push<i32>(const STRING& name, const i32& value)
            {
                m_attributes.push_back({ name, AttributeType::I32, value });
            }

            template<>
            void Push<VEC2>(const STRING& name, const VEC2& value)
            {
                m_attributes.push_back({ name, AttributeType::VEC2, value });
            }

            template<>
            void Push<VEC3>(const STRING& name, const VEC3& value)
            {
                m_attributes.push_back({ name, AttributeType::VEC3, value });
            }

            template<>
            void Push<VEC4>(const STRING& name, const VEC4& value)
            {
                m_attributes.push_back({ name, AttributeType::VEC4, value });
            }

            template<>
            void Push<MAT4>(const STRING& name, const MAT4& value)
            {
                m_attributes.push_back({ name, AttributeType::MAT4, value });
            }

            template<>
            void Push<TextureData>(const STRING& name, const TextureData& value)
            {
                m_attributes.push_back({ name, AttributeType::TEX2D, value });
            }

            void SetShader(Shader* shader);
            Shader* GetShader();

            void Bind() const;
        private:
            std::vector<MaterialAttribute> m_attributes;
            Shader* m_shader;
        };
    } // namespace Graphics
} // namespace CeosFramework
