#pragma once

#include "../definitions/typedefs.hpp"

namespace CPGFramework
{
    namespace Components
    {
        class Transform
        {
        public:
            Transform();
            ~Transform();

            const MAT4 GetLocalMatrix() const { return m_local; }
        private:
            MAT4 m_local;
        };
    } // namespace Components
} // namespace CPGFramework