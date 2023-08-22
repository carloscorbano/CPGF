#include "glBuffer.hpp"

namespace CPGFramework
{
    namespace Graphics
    {
        GLBuffer::GLBuffer(const void* data, const GLuint size, const GLenum target, const BufferUsage usage)
            : m_target(target), m_size(size), m_usage(usage)
        {
            GLCall(glGenBuffers(1, &m_ID));
            GLCall(glBindBuffer(target, m_ID));
            GLCall(glBufferData(target, size, data, (GLenum)usage));
        }

        GLBuffer::~GLBuffer()
        {
            GLCall(glDeleteBuffers(1, &m_ID));
        }

        /// @brief Bind the vertex buffer object.
        void GLBuffer::Bind() const 
        {
            GLCall(glBindBuffer(m_target, m_ID));
        }

        /// @brief Unbind the vertex buffer object.
        void GLBuffer::Unbind() const
        {
            GLCall(glBindBuffer(m_target, 0));
        }

        void* GLBuffer::MapBuffer(const BufferAccess access)
        {
            Bind();
            GLCall(return glMapBuffer(m_target, (GLenum)access));
        }

        void GLBuffer::UnmapBuffer()
        {
            GLCall(glUnmapBuffer(m_target));
        }

        void GLBuffer::BufferSubData(const GLuint offset, const GLuint size, const void* data)
        {
            Bind();
            GLCall(glBufferSubData(m_target, offset, size, data));
        }

        void GLBuffer::CopyBufferSubData(const GLBuffer read, const GLBuffer write, const GLuint readOffset, const GLuint writeOffset, const GLuint size)
        {
            GLCall(glBindBuffer(GL_COPY_READ_BUFFER, read.GetID()));
            GLCall(glBindBuffer(GL_COPY_WRITE_BUFFER, write.GetID()));
            GLCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size));
        }
    } // namespace Graphics
} // namespace CeosFramework