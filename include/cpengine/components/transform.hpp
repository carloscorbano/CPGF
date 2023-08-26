#pragma once

#include "../definitions/typedefs.hpp"
#include "../containers/data_tree.hpp"
#include <functional>

#define WORLD_UP        VEC3(0.0f, 1.0f, 0.0f)
#define WORLD_RIGHT     VEC3(1.0f, 0.0f, 0.0f)
#define WORLD_FORWARD   VEC3(0.0f, 0.0f, 1.0f)

namespace CPGFramework
{
    namespace World { class WorldClass; }
    namespace Components
    {
        enum class Space { LOCAL, WORLD };
        class Transform
        {
        public:
            friend class World::WorldClass;
            Transform();
            ~Transform();

            /// @brief Set this transform parent.
            /// @param parent 
            void SetParent(Transform& parent);
            void RemoveParent();
            /// @brief Get this transform parent, it'll return null if it don't have a parent.
            /// @return 
            Transform* GetParent();
            /// @brief Get the world transform matrix of this component.
            /// @return 
            const MAT4 GetTransformMatrix();
            /// @brief Converts this world transform matrix into a local transform matrix (relative to it's owner).
            /// @return 
            const MAT4 WorldToLocalTransformMatrix();

            /// @brief Translates this object, adding the amount in the axis direction.
            /// @param axis The move direction value.
            /// @param amount The amount to translate (it'll add to the current amount.)
            void Translate(const VEC3& axis, const FLOAT& amount, const Space& relativeTo = Space::WORLD);
            /// @brief Set the translation to the given value (it'll replace the current translation to the given value)
            /// @param value 
            void Translate(const VEC3& value);

            void Rotate(const VEC3& axis, const FLOAT& angle);
            /// @brief Set the transform scale.
            /// @param scale 
            void Scale(const VEC3& scale);

        private:
            void __INTERNAL__ApplyTranslationToChildrenRecursive(Containers::DataTree& hierarchy, Transform& current, const VEC3& value);
            void __INTERNAL__ApplyScaleToChildrenRecursive(Containers::DataTree& hierarchy, Transform& current, const VEC3& value);
        private:
            World::WorldClass* worldObj;
            Containers::DataTree::Node worldNode;
            
            MAT4 m_translation;
            MAT4 m_rotation;
            MAT4 m_scale;
            BOOL m_isDirty;

            MAT4 m_transform;
        };
    } // namespace Components
} // namespace CPGFramework