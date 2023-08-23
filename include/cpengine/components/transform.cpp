#include "transform.hpp"

namespace CPGFramework
{
    namespace Components
    {
        Transform::Transform()
            : m_local(MAT4_IDENTITY)
        {}

        Transform::~Transform()
        {}

        void Transform::SetParent(Transform& parent)
        {
            parentNode = parent.worldNode;
        }
    } // namespace Components
} // namespace CPGFramework
