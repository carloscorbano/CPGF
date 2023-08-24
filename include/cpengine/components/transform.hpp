#pragma once

#include "../definitions/typedefs.hpp"
#include "../containers/data_tree.hpp"
#include <functional>

namespace CPGFramework
{
    namespace World { class WorldClass; }
    namespace Components
    {
        class Transform
        {
        public:
            friend class World::WorldClass;
            Transform();
            ~Transform();

            void SetParent(Transform& parent);
            Transform* GetParent();

            const MAT4 GetTransformMatrix() const { return m_transform; }
            const MAT4 WorldToLocalTransformMatrix();

            void Translate(const VEC3& axis, const FLOAT& amount);
        private:
            void __INTERNAL__ApplyTranslationToChildrenRecursive(Containers::DataTree& hierarchy, Transform& current, const VEC3& value);
        private:
            World::WorldClass* worldObj;
            Containers::DataTree::Node worldNode;
            
            MAT4 m_transform;
        };
    } // namespace Components
} // namespace CPGFramework