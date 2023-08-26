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
            TextureData(Texture2D* texture, GLenum unit = GL_TEXTURE0) : texture(texture), unit(unit) {}
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

            template<typename T>
            T* GetAttribute(const STRING& name)
            {
                ASSERT(false);
            }

            template<typename T>
            std::vector<MaterialAttribute*> GetAllAttributesOfType()
            {
                ASSERT(false);
            }

            template<>
            std::vector<MaterialAttribute*> GetAllAttributesOfType<FLOAT>()
            {
                std::vector<MaterialAttribute*> result;
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.type == AttributeType::FLOAT)
                    {
                        result.push_back(&atr);
                    }
                }

                return result;
            }

            template<>
            FLOAT* GetAttribute<FLOAT>(const STRING& name)
            {
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.name == name && atr.type == AttributeType::FLOAT)
                    {
                        return std::any_cast<FLOAT*>(atr.value);
                    }
                }

                return nullptr;
            }

            template<>
            std::vector<MaterialAttribute*> GetAllAttributesOfType<i32>()
            {
                std::vector<MaterialAttribute*> result;
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.type == AttributeType::I32)
                    {
                        result.push_back(&atr);
                    }
                }
                
                return result;
            }

            template<>
            i32* GetAttribute<i32>(const STRING& name)
            {
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.name == name && atr.type == AttributeType::I32)
                    {
                        return std::any_cast<i32*>(atr.value);
                    }
                }

                return nullptr;
            }

            template<>
            std::vector<MaterialAttribute*> GetAllAttributesOfType<VEC2>()
            {
                std::vector<MaterialAttribute*> result;
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.type == AttributeType::VEC2)
                    {
                        result.push_back(&atr);
                    }
                }
                
                return result;
            }

            template<>
            VEC2* GetAttribute<VEC2>(const STRING& name)
            {
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.name == name && atr.type == AttributeType::VEC2)
                    {
                        return std::any_cast<VEC2*>(atr.value);
                    }
                }

                return nullptr;
            }

            template<>
            std::vector<MaterialAttribute*> GetAllAttributesOfType<VEC3>()
            {
                std::vector<MaterialAttribute*> result;
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.type == AttributeType::VEC3)
                    {
                        result.push_back(&atr);
                    }
                }
                
                return result;
            }

            template<>
            VEC3* GetAttribute<VEC3>(const STRING& name)
            {
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.name == name && atr.type == AttributeType::VEC3)
                    {
                        return std::any_cast<VEC3*>(atr.value);
                    }
                }

                return nullptr;
            }

            template<>
            std::vector<MaterialAttribute*> GetAllAttributesOfType<VEC4>()
            {
                std::vector<MaterialAttribute*> result;
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.type == AttributeType::VEC4)
                    {
                        result.push_back(&atr);
                    }
                }
                
                return result;
            }

            template<>
            VEC4* GetAttribute<VEC4>(const STRING& name)
            {
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.name == name && atr.type == AttributeType::VEC4)
                    {
                        return std::any_cast<VEC4*>(atr.value);
                    }
                }

                return nullptr;
            }

            template<>
            std::vector<MaterialAttribute*> GetAllAttributesOfType<MAT4>()
            {
                std::vector<MaterialAttribute*> result;
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.type == AttributeType::MAT4)
                    {
                        result.push_back(&atr);
                    }
                }
                
                return result;
            }

            template<>
            MAT4* GetAttribute<MAT4>(const STRING& name)
            {
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.name == name && atr.type == AttributeType::MAT4)
                    {
                        return std::any_cast<MAT4*>(atr.value);
                    }
                }

                return nullptr;
            }

            template<>
            std::vector<MaterialAttribute*> GetAllAttributesOfType<TextureData>()
            {
                std::vector<MaterialAttribute*> result;
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.type == AttributeType::TEX2D)
                    {
                        result.push_back(&atr);
                    }
                }
                
                return result;
            }

            template<>
            TextureData* GetAttribute<TextureData>(const STRING& name)
            {
                for(MaterialAttribute& atr : m_attributes)
                {
                    if(atr.name == name && atr.type == AttributeType::TEX2D)
                    {
                        return std::any_cast<TextureData*>(atr.value);
                    }
                }

                return nullptr;
            }


            BOOL Bind() const;
        private:
            std::vector<MaterialAttribute> m_attributes;
            Shader* m_shader;
        };
    } // namespace Graphics
} // namespace CeosFramework
