#include "transform.hpp"
#include "../modules/world/world.hpp"
#include "../modules/world/entity_node.hpp"
#include <glm/gtc/matrix_transform.hpp>
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
    } // namespace Components
} // namespace CPGFramework
