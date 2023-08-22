#pragma once

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"

namespace CPGFramework
{
    namespace Graphics
    {
        /// @brief A class that manages Vertex Array Objects (VAOs).
        class VertexArray 
        {
        public:
            VertexArray();
            ~VertexArray();
            /// @brief Adds a vertex buffer to this vao object with the given vertex buffer layout.
            /// @param vb The vertex buffer object.
            /// @param layout The Vertex Buffer Layout object.
            void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
            /// @brief Binds the vertex array object.
            void Bind();
            /// @brief Unbinds the vertex array object.
            void Unbind();
        private:
            /// @brief The unique ID of this object.
            GLuint m_rendererID;
        };
    } // namespace Graphics
} // namespace CPGFramework