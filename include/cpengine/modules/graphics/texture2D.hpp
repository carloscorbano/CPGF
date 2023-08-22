#pragma once

#include "glfw.hpp"
#include "../resources/resource_base.hpp"

namespace CPGFramework 
{
    namespace Graphics 
    {
        class Texture2D : public Resources::ResourceBase 
        {
        public:
            Texture2D(Engine* engine, const STRING& relativePath, const STRING& filename);
            ~Texture2D() override;

            void HotReload() override;

            void Bind(GLenum textureUnit = GL_TEXTURE0);
            ui32 GetInternalFormat() const;
            ui32 GetFormat() const;
            ui32 GetID() const;

            void SetupTexParameters(ui32 wrapping = GL_REPEAT, ui32 filterMin = GL_LINEAR_MIPMAP_LINEAR,
                ui32 filterMag = GL_LINEAR, BOOL bind = false);
            void AssignTextureData(i32 width, i32 height, i32 nChannels, const void* pixels, GLenum type = GL_UNSIGNED_BYTE, BOOL bind = false);
            void AssignSubTextureData(i32 xoffset, i32 yoffset, i32 width, i32 height, const void* pixels, GLenum type = GL_UNSIGNED_BYTE, BOOL bind = false);
        private:
            void __INTERNAL__ProcessPipelineCreation();

            void __INTERNAL__Setup(i32 width, i32 height, i32 nrChannels);
            void __INTERNAL__SetupInternalFormat(i32 numberOfChannels);
            void __INTERNAL__SetupInternalFormat(ui32 internalFormat, ui32 format);

        private:
            ui32 m_ID;
            i32 m_width, m_height, m_nrChannels;
            ui32 m_wrappingMode, m_filterMin, m_filterMag, m_internalFormat, m_format;
        };
    }
}