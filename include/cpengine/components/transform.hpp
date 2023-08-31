#pragma once

#include "../definitions/typedefs.hpp"
#include "../definitions/space.hpp"
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
            void SetParent(Transform* parent);
            /// @brief Get this transform parent, it'll return null if it don't have a parent.
            /// @return 
            Transform* GetParent();
            /// @brief Get the world transform matrix of this component.
            /// @return 
            const MAT4 GetWorldMatrix();

            /// @brief Teleports the object to the new position.
            /// @param newPosition 
            void Translate(const VEC3& newPosition);
            /// @brief Translates the object by the given axis and the amount, the axis is relative to the space
            /// @param axis The axis to be translated.
            /// @param amount The amount
            /// @param relativeTo The space relative to (local or world);
            void Translate(const VEC3& axis, const FLOAT& amount, const Space& relativeTo = Space::WORLD);
            /// @brief Rotates the object by the axis and angle (in degrees)
            /// @param axis The axis to be used as rotation axis.
            /// @param angle The angle (in degrees)
            /// @param relativeTo The space relative to (local or world).
            void Rotate(const VEC3& axis, const FLOAT& angle, const Space& relativeTo = Space::WORLD);
            /// @brief Set the rotation of the object (angles in degrees).
            /// @param rotation The new rotation.
            void Rotate(const VEC3& rotation);
            /// @brief Set the scale of the object.
            /// @param scale The new scale.
            void Scale(const VEC3& scale);
            /// @brief Get the local position. (use translate method to move the object).
            /// @return 
            const VEC3 GetLocalPosition() const;
            /// @brief Get the world position. (use translated method to move the object).
            /// @return 
            const VEC3 GetWorldPosition() const;
            /// @brief Get the local rotation. (use rotate methood to rotate the object).
            /// @return 
            const QUAT GetLocalRotation() const;
            /// @brief Get the world rotation. (use rotate method to rotate the object).
            /// @return 
            const QUAT GetWorldRotation() const;
            /// @brief Get the scale of the object (use scale method to scale the object).
            /// @return 
            const VEC3 GetScale() const;
            /// @brief Get The right vector (local axis).
            /// @return 
            const VEC3 GetRightVector() const;
            /// @brief Get The up vector (local axis).
            /// @return 
            const VEC3 GetUpVector() const;
            /// @brief Get The forward vector (local axis).
            const VEC3 GetForwardVector() const;

        private:
            void __INTERNAL__UpdateLocalMatrix();
        private:
            World::WorldClass* worldObj;
            Containers::DataTree::Node worldNode;      

            MAT4* toRootMatrix;
            MAT4 toWorldMatrix;
            MAT4 localMatrix;

            MAT4 localPosition;
            QUAT localRotation;
            MAT4 localScale;
        };
    } // namespace Components
} // namespace CPGFramework
