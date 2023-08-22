#pragma once

#include "glfw.hpp"
#include <vector>
#include <stdexcept>

namespace CPGFramework
{
    namespace Graphics
    {
        /// @brief Container for the vertex buffer element data.
        struct VertexBufferElement
        {
            GLuint type;
            GLuint count;
            GLboolean normalized;

            /// @brief Retrieve the size of the given type.
            /// @param type The type to get the size.
            /// @return 
            static GLuint GetSizeOfType(GLuint type) {
                switch (type)
                {
                    case GL_FLOAT:          return sizeof(GLfloat);
                    case GL_UNSIGNED_INT:   return sizeof(GLuint);
                    case GL_UNSIGNED_BYTE:  return sizeof(GLubyte);
                }

                ASSERT(false);
                return 0;
            }
        };

        /// @brief The vertex buffer layout. 
        /// Example usage is 'Push<GLfloat>(3)' (it'll insert into the layout 3 GLfloats).
        /// If the given vertex buffer layout is, for example, 3 floats 3 floats 2 floats (position, color and texCoord),
        /// just use 'Push<GLfloat>(3)' 'Push<GLfloat>(3)' 'Push<GLfloat>(2)'
        class VertexBufferLayout 
        {
        public:
            VertexBufferLayout() : m_stride(0) {}

            template<typename T>
            void Push(GLuint count)
            {
                throw std::runtime_error("Push generic template cannot be instanciated.");
            }

            /// @brief Push the given count of type GLfloat into the layout.
            /// @param count The number of elements of type float in the vertex buffer
            template<>
            void Push<GLfloat>(GLuint count)
            {
                m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
                m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
            }

            /// @brief Push the given count of type GLuint into the layout.
            /// @param count The number of elements of type GLuint in the vertex buffer
            template<>
            void Push<GLuint>(GLuint count)
            {
                m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
                m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
            }

            /// @brief Push the given count of type GLubyte into the layout.
            /// @param count The number of elements of type GLubyte in the vertex buffer
            template<>
            void Push<GLubyte>(GLuint count)
            {
                m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
                m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
            }

            /// @brief Get the layout elements vector.
            /// @return 
            inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
            /// @brief Get the stride of this layout.
            /// @return 
            inline GLuint GetStride() const { return m_stride; }
        private:
            /// @brief This layout elements
            std::vector<VertexBufferElement> m_elements;
            /// @brief This layout stride.
            GLuint m_stride;
        };
    } // namespace Graphics
} // namespace CPGFramework
