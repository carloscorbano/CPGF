#pragma once

#include "vertex.hpp"
#include "vertex_array.hpp"
#include "index_buffer.hpp"
#include "material.hpp"

namespace CPGFramework
{
    namespace Graphics
    {
        class Mesh
        {
        public:
            Mesh();
            ~Mesh();

            void AddTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);
            void AddQuad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3);
            void Compile();

            inline void SetVertices(const std::vector<Vertex>& vertices)    { m_vertices = vertices;    }
            inline void SetIndices(const std::vector<GLuint>& indices)      { m_indices = indices;      }

            inline Material& GetMaterial() { return m_mat; }

            BOOL Bind();
            void Draw(const BOOL& keepBounded = false);
        private:
            void DeleteBuffers();
        private:
            std::vector<Vertex> m_vertices;
            std::vector<GLuint> m_indices;

            Material m_mat;
            VertexArray* m_va;
            VertexBuffer* m_vb;
            IndexBuffer* m_ib;
            BOOL m_compiled;
            BOOL m_bounded;
        };
    } // namespace Graphics
} // namespace CPGFramework