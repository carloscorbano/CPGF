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

            /// @brief Set this transform parent.
            /// @param parent 
            void SetParent(Transform& parent);
            /// @brief Get this transform parent, it'll return null if it don't have a parent.
            /// @return 
            Transform* GetParent();
            /// @brief Get the world transform matrix of this component.
            /// @return 
            inline const MAT4 GetTransformMatrix() const { return m_transform; }
            /// @brief Converts this world transform matrix into a local transform matrix (relative to it's owner).
            /// @return 
            const MAT4 WorldToLocalTransformMatrix();

            /// @brief Translates this object, adding the amount in the axis direction.
            /// @param axis The move direction value.
            /// @param amount The amount to translate (it'll add to the current amount.)
            void Translate(const VEC3& axis, const FLOAT& amount);
            /// @brief Set the translation to the given value (it'll replace the current translation to the given value)
            /// @param value 
            void Translate(const VEC3& value);

            void Rotate(const VEC3& axis, const FLOAT& value);
            /// @brief Set the transform scale.
            /// @param scale 
            void Scale(const VEC3& scale);

        private:
            void __INTERNAL__ApplyTranslationToChildrenRecursive(Containers::DataTree& hierarchy, Transform& current, const VEC3& value);
            void __INTERNAL__ApplyScaleToChildrenRecursive(Containers::DataTree& hierarchy, Transform& current, const VEC3& value);
        private:
            World::WorldClass* worldObj;
            Containers::DataTree::Node worldNode;
            
            MAT4 m_transform;
        };
    } // namespace Components
} // namespace CPGFramework