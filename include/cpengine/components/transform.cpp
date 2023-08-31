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
            :   worldObj(nullptr), toRootMatrix(nullptr), toWorldMatrix(MAT4_IDENTITY), localMatrix(MAT4_IDENTITY)
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
            localMatrix = GetWorldMatrix();

            //set the new root matrix
            toRootMatrix = &target->toWorldMatrix;

            //apply the inverse of the new root matrix, to keep position, rotate and scale.
            localMatrix = glm::inverse(*toRootMatrix) * localMatrix;
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
            localMatrix[3].x = newPosition.x;
            localMatrix[3].y = newPosition.y;
            localMatrix[3].z = newPosition.z;
        }

        void Transform::Translate(const VEC3& axis, const FLOAT& amount, const Space& relativeTo)
        {
            VEC3 resultAxis = __INTERNAL__CalculateResultAxis(axis, relativeTo);
            localMatrix = glm::translate(localMatrix, amount * resultAxis);
        }

        void Transform::Rotate(const VEC3& axis, const FLOAT& angle, const Space& relativeTo)
        {
            VEC3 resultAxis = __INTERNAL__CalculateResultAxis(axis, relativeTo);
            localMatrix = glm::rotate(localMatrix, DegToRad(angle), resultAxis);
        }

        void Transform::Rotate(const VEC3& rotation)
        {
            localMatrix *= glm::toMat4(__INTERNAL__GetQuatRot(true));
            localMatrix *= glm::toMat4(QUAT(rotation));
        }

        void Transform::Scale(const VEC3& scale)
        {
            //reset scale
            VEC3 oldScale = GetScale();
            localMatrix[0] /= oldScale.x;
            localMatrix[1] /= oldScale.y;
            localMatrix[2] /= oldScale.z;

            //set new scale
            localMatrix[0] *= scale.x;
            localMatrix[1] *= scale.y;
            localMatrix[2] *= scale.z;
        }

        const VEC3 Transform::GetLocalPosition() const
        {
            return localMatrix[3];
        }

        const VEC3 Transform::GetWorldPosition() const
        {
            return toWorldMatrix[3];
        }

        const QUAT Transform::GetLocalRotation()
        {
            return __INTERNAL__GetQuatRot();
        }

        const QUAT Transform::GetWorldRotation() const
        {
            return glm::quat_cast(toWorldMatrix);
        }

        const VEC3 Transform::GetScale() const
        {
            return VEC3(glm::length(VEC3(localMatrix[0])), glm::length(VEC3(localMatrix[1])), glm::length(VEC3(localMatrix[2])));
        }

        const VEC3 Transform::GetRightVector()
        {
            return __INTERNAL__GetQuatRot() * VEC3_RIGHT;
        }

        const VEC3 Transform::GetUpVector()
        {
            return __INTERNAL__GetQuatRot() * VEC3_UP;
        }

        const VEC3 Transform::GetForwardVector()
        {
            return __INTERNAL__GetQuatRot() * VEC3_FORWARD;
        }

        VEC3 Transform::__INTERNAL__CalculateResultAxis(const VEC3& axis, const Space& relativeTo)
        {
            VEC3 normalized = glm::normalize(axis);
            switch (relativeTo)
            {
                case Space::WORLD:  return __INTERNAL__GetQuatRot(true) * normalized;
                case Space::LOCAL:  return normalized;
                default:            return VEC3_ZERO;
            }
        }

        QUAT Transform::__INTERNAL__GetQuatRot(const BOOL& inverse)
        {
            QUAT result = glm::toQuat(localMatrix);
            return inverse ? glm::inverse(result) : result;
        }
    } // namespace Components
} // namespace CPGFramework
