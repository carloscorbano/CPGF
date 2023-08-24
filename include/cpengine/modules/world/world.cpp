#include "world.hpp"
#include "../../components/transform.hpp"
#include "../../debug/debug.hpp"
#include "entity.hpp"
#include "entity_node.hpp"

namespace CPGFramework
{
    namespace World
    {
        WorldClass::WorldClass(Engine* engine)
            : IEngineModule(engine)
        {
            __INTERNAL__SetupHierarchyRoot();
            __INTERNAL__BindCallbacks();
        }

        WorldClass::~WorldClass()
        {}

        void WorldClass::__INTERNAL__SetupHierarchyRoot()
        {
            auto rootNode = m_hierarchy.GetRoot();
            EntityNode* data = m_hierarchy.CreateData<EntityNode>(rootNode);
            data->id = m_registry.create();
            auto& transform = m_registry.emplace<Components::Transform>(data->id);
            transform.worldNode = rootNode;
        }

        void WorldClass::__INTERNAL__BindCallbacks()
        {
            Entity::worldRegistry = &m_registry;
            
            Entity::onConstructEntityRequest = [&]() { return OnConstructEntityRequestedCallback(); };
            Entity::onDestroyRequest = [&](entt::entity entity){ OnDestroyEntityRequestedCallback(entity); };

            m_registry.on_construct<Components::Transform>().connect<&WorldClass::OnTransformComponentAdded>(this);
            m_registry.on_destroy<Components::Transform>().connect<&WorldClass::OnTransformComponentDestroyed>(this);

            // Components::Transform::ReqEnqueueParent = [&](Containers::DataTree::Node child, Containers::DataTree::Node owner)
            // {
            //     toParentNodes.push({ child, owner });
            // };

            // Components::Transform::ReqParentNode = [&](Containers::DataTree::Node node)
            // {
            //     return m_hierarchy.GetNodeOwner(node);
            // };

            // Components::Transform::ReqNodeTransformData = [&](Containers::DataTree::Node fromNode)
            // {
            //     EntityNode* en = m_hierarchy.GetData<EntityNode>(fromNode);
            //     return m_registry.try_get<Components::Transform>(en->id);
            // };

            // Components::Transform::ReqChildrenIterator = [&](Containers::DataTree::Node node, std::function<BOOL(Components::Transform& childTransform)> operation)
            // {
            //     m_hierarchy.ViewChildren<EntityNode>(node, [&](Containers::DataTree& tree, Containers::DataTree::Node& node, EntityNode& data)
            //     {
            //         Components::Transform* childTransform = m_registry.try_get<Components::Transform>(data.id);
            //         return operation(*childTransform);
            //     });
            // };
        }

        void WorldClass::OnTransformComponentAdded(entt::registry& reg, entt::entity entity)
        {
            Containers::DataTree::Node node = m_hierarchy.CreateNode();
            EntityNode* nodeData = m_hierarchy.CreateData<EntityNode>(node);
            nodeData->id = entity;

            auto& transform = m_registry.get<Components::Transform>(entity);
            transform.worldObj = this;
            transform.worldNode = node;
            //queue to be added to hierarchy tree.
            toAddToHierarchy.push(entity);
        }

        void WorldClass::OnTransformComponentDestroyed(entt::registry& reg, entt::entity entity)
        {
            auto& transform = m_registry.get<Components::Transform>(entity);
            Containers::DataTree::Node node = transform.worldNode;
            toRemoveFromHierarchy.push(node);
        }

        entt::entity WorldClass::OnConstructEntityRequestedCallback()
        {
            return m_registry.create();
        }

        void WorldClass::OnDestroyEntityRequestedCallback(entt::entity entity)
        {
            toDestroyEntity.push(entity);
        }

        void WorldClass::Update()
        {
            //process queues
            while(!toAddToHierarchy.empty())
            {
                entt::entity e = toAddToHierarchy.front();
                toAddToHierarchy.pop();

                if(m_registry.valid(e))
                {
                    auto* transform = m_registry.try_get<Components::Transform>(e);
                    if(transform && !m_hierarchy.HasOwner(transform->worldNode))
                    {
                        m_hierarchy.SetNodeOwner(transform->worldNode, m_hierarchy.GetRoot());
                    }
                }
            }

            while(!toRemoveFromHierarchy.empty())
            {
                Containers::DataTree::Node n = toRemoveFromHierarchy.front();
                toRemoveFromHierarchy.pop();

                DEBUG_LOG("REMOVING FROM HIERARCHY NODE", n.GetIndex());
                m_hierarchy.FreeNode(n);
            }

            while(!toDestroyEntity.empty())
            {
                entt::entity e = toDestroyEntity.front();
                toDestroyEntity.pop();

                DEBUG_LOG("DESTROYING ENTITY ID", (ui32)e);
                m_registry.destroy(e);
            }
        }
    } // namespace World
} // namespace CPGFramework
