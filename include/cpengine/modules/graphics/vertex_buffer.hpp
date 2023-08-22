#pragma once

#include "glBuffer.hpp"
namespace CPGFramework
{
    namespace Graphics
    {
        /// @brief Vertex buffer Object (VBO) manager class. It helps to create VBOs and manage it.
        class VertexBuffer : public GLBuffer
        {
        public:
            /// @brief Default constructor.
            /// @param data The given vertex buffer data (to be assigned to the buffer data).
            /// @param size The size of the object (example: dataElementsCount * sizeof(GLfloat)).
            VertexBuffer(const void* data, const GLuint size, const BufferUsage usage = BufferUsage::STATIC_DRAW)
                : GLBuffer(data, size, GL_ARRAY_BUFFER, usage) {}
        };
    } // namespace Graphics
} // namespace CPGFramework