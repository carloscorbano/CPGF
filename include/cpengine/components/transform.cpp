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
    } // namespace Components
} // namespace CPGFramework
