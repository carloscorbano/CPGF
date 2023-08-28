#include "transform.hpp"
#include "../modules/world/world.hpp"
#include "../modules/world/entity_node.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "../definitions/stringify.hpp"

namespace CPGFramework
{
    namespace Components
    {
        Transform::Transform()
            :   worldObj(nullptr), m_transform(MAT4_IDENTITY), m_translation(MAT4_IDENTITY),
                m_rotation(QUAT(VEC3_ZERO)), m_scale(MAT4_IDENTITY), m_isDirty(true)
        {
            GetTransformMatrix();
        }

        Transform::~Transform()
        {}

        const MAT4 Transform::GetTransformMatrix()
        { 
            if(m_isDirty)
            {
                m_isDirty = false;
                m_transform = m_translation * glm::toMat4(m_rotation) * m_scale;
            }

            return m_transform; 
        }

        void Transform::SetParent(Transform& parent)
        {
            worldObj->GetHierarchy().SetNodeOwner(worldNode, parent.worldNode);
        }

        void Transform::RemoveParent()
        {
            auto& hierarchy = worldObj->GetHierarchy();
            hierarchy.SetNodeOwner(worldNode, hierarchy.GetRoot());
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

        const MAT4 Transform::WorldToLocalTransformMatrix()
        {
            Transform* parent = GetParent();
            if(!parent)
            {
                return GetTransformMatrix();
            }

            return GetTransformMatrix() * glm::inverse(parent->GetTransformMatrix());
        }

        void Transform::Translate(const VEC3& axis, const FLOAT& amount, const Space& relativeTo)
        {
            VEC3 result = VEC3_ZERO;
            switch (relativeTo)
            {
            case Space::LOCAL:
            {
                result = (m_rotation * axis) * amount;
            } break;
            case Space::WORLD:
            {
                result = axis * amount;
            } break;
            default: break;
            }

            auto& hierarchy = worldObj->GetHierarchy();
            __INTERNAL__ApplyTranslationToChildrenRecursive(hierarchy, *this, result);
        }

        void Transform::TranslateTowards(const VEC3& target, const FLOAT& amount)
        {
            VEC4 translation = m_translation[3];

        }

        void Transform::Translate(const VEC3& value)
        {
            MAT4 result = MAT4_IDENTITY;
            result = glm::translate(result, value);
            MAT4 delta = result * glm::inverse(m_translation);
            VEC3 amount{};
            amount.x = delta[3].x;
            amount.y = delta[3].y;
            amount.z = delta[3].z;

            auto& hierarchy = worldObj->GetHierarchy();
            __INTERNAL__ApplyTranslationToChildrenRecursive(hierarchy, *this, amount);
        }

        void Transform::Rotate(const VEC3& axis, const FLOAT& angle)
        {
            m_rotation = glm::rotate(m_rotation, glm::radians(angle), axis);
            m_isDirty = true;
        }

        void Transform::Scale(const VEC3& scale)
        {
            //TODO: make scale in children move it from the owner.
            auto& hierarchy = worldObj->GetHierarchy();
            __INTERNAL__ApplyScaleToChildrenRecursive(hierarchy, *this, scale);
        }

        const VEC3 Transform::GetPosition()
        {
            return m_translation[3];
        }

        const VEC3 Transform::GetEulerRotation()
        {
            return VEC3();
        }

        const VEC3 Transform::GetScale()
        {
            return VEC3(glm::length(VEC3(m_scale[0])), glm::length(VEC3(m_scale[1])), glm::length(VEC3(m_scale[2])));
        }

        const VEC3 Transform::GetUpVector()
        {
            return m_rotation * VEC3_UP;
        }

        const VEC3 Transform::GetRightVector()
        {
            return m_rotation * VEC3_RIGHT;
        }

        const VEC3 Transform::GetForwardVector()
        {
            return m_rotation * VEC3_FORWARD;
        }

        void Transform::__INTERNAL__ApplyTranslationToChildrenRecursive(Containers::DataTree& hierarchy, Transform& current, const VEC3& value)
        {
            current.m_isDirty = true;
            current.m_translation = glm::translate(current.m_translation, value);

            hierarchy.ViewChildren<World::EntityNode>(current.worldNode, 
            [&](Containers::DataTree& tree, Containers::DataTree::Node& node, World::EntityNode& en)
            {
                Transform* next = worldObj->GetRegistry().try_get<Transform>(en.id);
                if(next)
                {
                    __INTERNAL__ApplyTranslationToChildrenRecursive(hierarchy, *next, value);
                }
                return true;
            });
        }

        void Transform::__INTERNAL__ApplyScaleToChildrenRecursive(Containers::DataTree& hierarchy, Transform& current, const VEC3& value)
        {
            DecomposedMAT4 decomposed(current.GetTransformMatrix());
            MAT4 translation = glm::translate(MAT4_IDENTITY, decomposed.translation);
            MAT4 rotation = glm::toMat4(decomposed.orientation);
            MAT4 scale = glm::scale(MAT4_IDENTITY, value);

            current.m_transform = translation * rotation * scale;

            hierarchy.ViewChildren<World::EntityNode>(current.worldNode, 
            [&](Containers::DataTree& tree, Containers::DataTree::Node& node, World::EntityNode& en)
            {
                Transform* next = worldObj->GetRegistry().try_get<Transform>(en.id);
                if(next)
                {
                    __INTERNAL__ApplyScaleToChildrenRecursive(hierarchy, *next, value);
                }
                return true;
            });
        }
    } // namespace Components
} // namespace CPGFramework
