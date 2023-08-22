#include "vertex_array.hpp"

namespace CPGFramework
{
    namespace Graphics
    {
        VertexArray::VertexArray() 
        {
            GLCall(glGenVertexArrays(1, &m_rendererID));
        }

        VertexArray::~VertexArray() 
        {
            GLCall(glDeleteVertexArrays(1, &m_rendererID));            
        }

        void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) 
        {
            Bind();
            vb.Bind();

            const std::vector<VertexBufferElement>& elements = layout.GetElements();
            GLuint offset = 0;
            for(GLuint i = 0; i < elements.size(); ++i)
            {
                const VertexBufferElement& element = elements[i];
                GLCall(glEnableVertexAttribArray(i));
                GLCall(glVertexAttribPointer(i, element.count, element.type, 
                        element.normalized, layout.GetStride(), (const void*)(uintptr_t)(offset)));

                offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
            }
        }

        void VertexArray::Bind()
        {
            GLCall(glBindVertexArray(m_rendererID));
        }

        void VertexArray::Unbind()
        {
            GLCall(glBindVertexArray(0));
        }
    } // namespace Graphics
} // namespace CeosFramework