#include "transform.hpp"
#include "../definitions/stringify.hpp"
#include "../definitions/constants.hpp"
#include "../definitions/mat4.hpp"
#include "../definitions/math.hpp"

#include "../debug/debug.hpp"

#include "../modules/world/world.hpp"
#include "../modules/world/entity_node.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace CPGFramework
{
    namespace Components
    {
        Transform::Transform()
            :   worldObj(nullptr), toRootMatrix(nullptr), toWorldMatrix(MAT4_IDENTITY), localMatrix(MAT4_IDENTITY),
                localPosition(MAT4_IDENTITY), localRotation(QUAT(VEC3(0))), localScale(MAT4_IDENTITY)
        {}

        Transform::~Transform()
        {}

        void Transform::SetParent(Transform* parent)
        {
            Transform* target = parent;
            if(target == this) return;

            if(parent)
            {
                worldObj->GetHierarchy().SetNodeOwner(worldNode, parent->worldNode);
            }
            else
            {
                auto& hierarchy = worldObj->GetHierarchy();
                hierarchy.SetNodeOwner(worldNode, hierarchy.GetRoot());

                auto* nodeData = hierarchy.GetData<World::EntityNode>(hierarchy.GetRoot());
                auto& transform = worldObj->GetRegistry().get<Transform>(nodeData->id);
                target = &transform;
            }

            //apply to local matrix, to keep the changes from the parent.
            DecomposedMAT4 oldMat(GetWorldMatrix());
            Translate(oldMat.position);
            localRotation = oldMat.rotation;
            Scale(oldMat.scale);
            __INTERNAL__UpdateLocalMatrix();

            //set the new root matrix
            toRootMatrix = &target->toWorldMatrix;

            //apply the inverse of the new root matrix, to keep position, rotate and scale.
            MAT4 result = glm::inverse(*toRootMatrix) * localMatrix;
            DecomposedMAT4 resultDec(result);
            Translate(resultDec.position);
            localRotation = resultDec.rotation;
            Scale(resultDec.scale);
            __INTERNAL__UpdateLocalMatrix();
        }

        Transform* Transform::GetParent()
        {
            Transform* result = nullptr;
            auto& hierarchy = worldObj->GetHierarchy();
            Containers::DataTree::Node owner = hierarchy.GetNodeOwner(worldNode);
            if(owner.IsValid() && !owner.IsRoot())
            {
                World::EntityNode* en = hierarchy.GetData<World::EntityNode>(owner);
                result = worldObj->GetRegistry().try_get<Transform>(en->id);
            }

            return result;
        }

        const MAT4 Transform::GetWorldMatrix()
        {
            toWorldMatrix = MAT4_IDENTITY;

            if(toRootMatrix)
            {
                toWorldMatrix = (*toRootMatrix) * localMatrix;
            }

            return toWorldMatrix;
        }

        void Transform::Translate(const VEC3& newPosition)
        {
            localPosition = glm::translate(MAT4_IDENTITY, newPosition);
            __INTERNAL__UpdateLocalMatrix();
        }

        void Transform::Translate(const VEC3& axis, const FLOAT& amount, const Space& relativeTo)
        {
            VEC3 resultAxis = VEC3_ZERO;
            switch (relativeTo)
            {
            case Space::WORLD:  resultAxis = glm::normalize(axis);                  break;
            case Space::LOCAL:  resultAxis = localRotation * glm::normalize(axis);  break;
            }

            localPosition = glm::translate(localPosition, amount * resultAxis);
            __INTERNAL__UpdateLocalMatrix();
        }

        void Transform::Rotate(const VEC3& axis, const FLOAT& angle, const Space& relativeTo)
        {
            VEC3 resultAxis = VEC3_ZERO;
            switch (relativeTo)
            {
            case Space::LOCAL:  resultAxis = glm::normalize(axis);                                  break;
            case Space::WORLD:  resultAxis = glm::inverse(localRotation) * glm::normalize(axis);    break;
            }

            localRotation = glm::rotate(localRotation, DegToRad(angle), resultAxis);
            __INTERNAL__UpdateLocalMatrix();
        }

        void Transform::Rotate(const VEC3& rotation)
        {
            localRotation = QUAT(rotation);
            __INTERNAL__UpdateLocalMatrix();
        }

        void Transform::Scale(const VEC3& scale)
        {
            localScale = glm::scale(MAT4_IDENTITY, scale);
            __INTERNAL__UpdateLocalMatrix();
        }

        const VEC3 Transform::GetLocalPosition() const
        {
            return localMatrix[3];
        }

        const VEC3 Transform::GetWorldPosition() const
        {
            return toWorldMatrix[3];
        }

        const QUAT Transform::GetLocalRotation() const
        {
            return localRotation;
        }

        const QUAT Transform::GetWorldRotation() const
        {
            return glm::quat_cast(toWorldMatrix);
        }

        const VEC3 Transform::GetScale() const
        {
            return VEC3(
                glm::length(VEC3(localMatrix[0])), 
                glm::length(VEC3(localMatrix[1])), 
                glm::length(VEC3(localMatrix[2]))
                );
        }

        const VEC3 Transform::GetRightVector() const
        {
            return localRotation * VEC3_RIGHT;
        }
        const VEC3 Transform::GetUpVector() const
        {
            return localRotation * VEC3_UP;
        }
        const VEC3 Transform::GetForwardVector() const
        {
            return localRotation * VEC3_FORWARD;
        }

        void Transform::__INTERNAL__UpdateLocalMatrix()
        {
            localMatrix = localPosition * glm::toMat4(localRotation) * localScale;
        }
    } // namespace Components
} // namespace CPGFramework
