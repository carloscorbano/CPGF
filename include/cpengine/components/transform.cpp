#include "transform.hpp"
#include "../modules/world/world.hpp"
#include "../modules/world/entity_node.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../definitions/stringify.hpp"
namespace CPGFramework
{
    namespace Components
    {
        Transform::Transform()
            : worldObj(nullptr), m_transform(MAT4_IDENTITY)
        {}

        Transform::~Transform()
        {}

        void Transform::SetParent(Transform& parent)
        {
            worldObj->GetHierarchy().SetNodeOwner(worldNode, parent.worldNode);
        }

        Transform* Transform::GetParent()
        {
            Transform* result = nullptr;
            auto& hierarchy = worldObj->GetHierarchy();
            Containers::DataTree::Node owner = hierarchy.GetNodeOwner(worldNode);
            if(owner.IsValid())
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

        void Transform::Translate(const VEC3& axis, const FLOAT& amount)
        {
            auto& hierarchy = worldObj->GetHierarchy();
            __INTERNAL__ApplyTranslationToChildrenRecursive(hierarchy, *this, axis * amount);
        }

        void Transform::Translate(const VEC3& value)
        {
            MAT4 result = MAT4_IDENTITY;
            result = glm::translate(result, value);
            MAT4 delta = result * glm::inverse(m_transform);
            VEC3 amount{};
            amount.x = delta[3].x;
            amount.y = delta[3].y;
            amount.z = delta[3].z;

            auto& hierarchy = worldObj->GetHierarchy();
            __INTERNAL__ApplyTranslationToChildrenRecursive(hierarchy, *this, amount);
        }

        void Transform::Rotate(const VEC3& axis, const FLOAT& value)
        {

        }

        void Transform::Scale(const VEC3& scale)
        {
            //TODO: make scale in children move it from the owner.
            auto& hierarchy = worldObj->GetHierarchy();
            __INTERNAL__ApplyScaleToChildrenRecursive(hierarchy, *this, scale);
        }

        void Transform::__INTERNAL__ApplyTranslationToChildrenRecursive(Containers::DataTree& hierarchy, Transform& current, const VEC3& value)
        {
            current.m_transform = glm::translate(current.m_transform, value);
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
