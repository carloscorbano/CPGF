#pragma once

#include "glfw.hpp"
#include "glTypes.hpp"

namespace CPGFramework
{
    namespace Graphics
    {
        class GLBuffer
        {
        public:
            /// @brief Default buffer constructor method.
            /// @param data The data to be inserted into the buffer.
            /// @param size The size of the data.
            /// @param target The buffer target (example: 'GL_ELEMENT_ARRAY_BUFFER').
            /// @param usage This buffer type of usage.
            GLBuffer(const void* data, const GLuint size, const GLenum target, const BufferUsage usage = BufferUsage::STATIC_DRAW);
            ~GLBuffer();
            /// @brief Bind the vertex buffer object.
            void Bind() const;
            /// @brief Unbind the vertex buffer object.
            void Unbind() const;
            /// @brief Maps the buffer and return the pointer access to the data.
            /// @param access The access type.
            void* MapBuffer(const BufferAccess access = BufferAccess::WRITE_ONLY);
            /// @brief After finished with the map buffer, call this to unmap it.
            void UnmapBuffer();
            /// @brief Set the buffer sub data.
            /// @param offset The offset of the data.
            /// @param size The size of the data.
            /// @param data The data to be setted.
            void BufferSubData(const GLuint offset, const GLuint size, const void* data);
            /// @brief Copy the buffer read data to to write buffer data.
            /// @param read The buffer that will have the data copied
            /// @param write The buffer that will receive the data.
            /// @param readOffset The offset from the read data buffer.
            /// @param writeOffset The offset that the write buffer will receive the data.
            /// @param size The size of the data that will be copied.
            void CopyBufferSubData(const GLBuffer read, const GLBuffer write, const GLuint readOffset, const GLuint writeOffset, const GLuint size);
            /// @brief Get this buffer ID.
            inline const GLuint GetID()                 const { return m_ID;        }
            /// @brief Get this buffer target.
            inline const GLenum GetTarget()             const { return m_target;    }
            /// @brief Get this buffer size.
            inline const GLuint GetSize()               const { return m_size;      }
            /// @brief Get this buffer usage type.
            inline const BufferUsage GetBufferUsage()   const { return m_usage;     }
        private:
            GLuint m_ID;
            GLenum m_target;
            GLuint m_size;
            BufferUsage m_usage;
        };
    } // namespace Graphics
} // namespace CeosFramework