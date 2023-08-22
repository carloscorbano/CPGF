#pragma once

#include "glBuffer.hpp"
namespace CPGFramework
{
    namespace Graphics
    {
        /// @brief Index buffer container object. It manages 'GL_ELEMENT_ARRAY_BUFFER' abstraction.
        ///         Class that holdes indices.
        class IndexBuffer : public GLBuffer
        {
        public:
            /// @brief Default constructor.
            /// @param data The data array that contains the indices to be inserted into the buffer data.
            /// @param count The number of elements in the data array.
            IndexBuffer(const GLuint* data, const GLuint count, const BufferUsage usage = BufferUsage::STATIC_DRAW)
                : GLBuffer((const void*)data, count * sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER, usage), m_count(count) {}

            /// @brief Get the number of elements in this buffer.
            /// @return 
            inline const GLuint GetCount() const { return m_count; }
        private:
            /// @brief The number of elements in this buffer object.
            GLuint m_count;
        };
    } // namespace Graphics
} // namespace CeosFramework