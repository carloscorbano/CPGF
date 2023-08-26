#include "material.hpp"

namespace CPGFramework
{
    namespace Graphics
    {
        Material::Material() {}

        Material::~Material() {}

        BOOL Material::Bind() const
        {
            if(m_shader && m_shader->GetResourceState() == Resources::ResourceState::OK)
            {
                m_shader->Bind();

                for(const MaterialAttribute& attribute : m_attributes)
                {
                    switch (attribute.type)
                    {
                    case AttributeType::FLOAT:
                    {
                        m_shader->SetUniform<FLOAT>(attribute.name, std::any_cast<FLOAT>(attribute.value));
                    } break;
                    case AttributeType::I32:
                    {
                        m_shader->SetUniform<i32>(attribute.name, std::any_cast<i32>(attribute.value));
                    } break;
                    case AttributeType::MAT4:
                    {
                        m_shader->SetUniform<MAT4>(attribute.name, std::any_cast<MAT4>(attribute.value));
                    } break;
                    case AttributeType::VEC2:
                    {
                        m_shader->SetUniform<VEC2>(attribute.name, std::any_cast<VEC2>(attribute.value));
                    } break;
                    case AttributeType::VEC3:
                    {
                        m_shader->SetUniform<VEC3>(attribute.name, std::any_cast<VEC3>(attribute.value));
                    } break;
                    case AttributeType::VEC4:
                    {
                        m_shader->SetUniform<VEC4>(attribute.name, std::any_cast<VEC4>(attribute.value));
                    } break;
                    case AttributeType::TEX2D:
                    {
                        TextureData texData = std::any_cast<TextureData>(attribute.value);
                        if(texData.texture->GetResourceState() == Resources::ResourceState::OK)
                        {
                            texData.texture->Bind(texData.unit);
                        }
                    } break;
                    default:
                        break;
                    }
                }

                return true;
            }

            return false;
        }

        void Material::SetShader(Shader* shader)
        {
            m_shader = shader;
        }

        Shader* Material::GetShader()
        {
            return m_shader;
        }
    } // namespace Graphics
} // namespace CeosFramework
