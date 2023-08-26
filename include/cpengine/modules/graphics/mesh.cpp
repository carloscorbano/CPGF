#include "mesh.hpp"

namespace CPGFramework
{
    namespace Graphics
    {
        Mesh::Mesh()
            : m_va(nullptr), m_vb(nullptr), m_ib(nullptr), m_compiled(false), m_bounded(false)
        {}

        Mesh::~Mesh()
        {
            DeleteBuffers();
        }

        void Mesh::AddTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
        {
            m_indices.push_back(m_vertices.size());
            m_vertices.push_back(v0);

            m_indices.push_back(m_vertices.size());
            m_vertices.push_back(v1);

            m_indices.push_back(m_vertices.size());
            m_vertices.push_back(v2);
        }

        void Mesh::AddQuad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3)
        {
            AddTriangle(v0, v1, v2);
            AddTriangle(v1, v3, v2);
        }

        void Mesh::Compile()
        {
            m_compiled = false;
            DeleteBuffers();

            m_va = new VertexArray();
            m_vb = new VertexBuffer(m_vertices.data(), m_vertices.size() * sizeof(Vertex));
            m_ib = new IndexBuffer(m_indices.data(), m_indices.size());

            VertexBufferLayout vbl;
            vbl.Push<GLfloat>(3);
            vbl.Push<GLfloat>(3);
            vbl.Push<GLfloat>(2);

            m_va->AddBuffer(*m_vb, vbl);
            m_compiled = true;
        }


        BOOL Mesh::Bind()
        {
            if(m_compiled && m_mat.Bind())
            {
                m_va->Bind();
                m_ib->Bind();
                m_bounded = true;

                return true;
            }

            return false;
        }

        void Mesh::Draw(const BOOL& keepBounded)
        {
            if(m_compiled && m_bounded)
            {
                glDrawElements(GL_TRIANGLES, m_ib->GetCount(), GL_UNSIGNED_INT, 0);
                if(!keepBounded) m_bounded = false;
            }
        }

        void Mesh::DeleteBuffers()
        {
            if(m_va) delete m_va;
            if(m_vb) delete m_vb;
            if(m_ib) delete m_ib;
        }
    } // namespace Graphics
} // namespace CPGFramework