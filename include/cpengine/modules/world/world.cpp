#include "world.hpp"
#include "../../components/transform.hpp"
#include "../../debug/debug.hpp"

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
            m_registry.emplace<Components::Transform>(data->id);
        }

        void WorldClass::__INTERNAL__BindCallbacks()
        {
            Entity::worldRegistry = &m_registry;
            
            Entity::onConstructEntityRequest = [&]() { return OnConstructEntityRequestedCallback(); };
            Entity::onDestroyRequest = [&](entt::entity entity){ OnDestroyEntityRequestedCallback(entity); };

            m_registry.on_construct<Components::Transform>().connect<&WorldClass::OnTransformComponentAdded>(this);
            m_registry.on_destroy<Components::Transform>().connect<&WorldClass::OnTransformComponentDestroyed>(this);
        }

        void WorldClass::OnTransformComponentAdded(entt::registry& reg, entt::entity entity)
        {
            Containers::DataTree::Node node = m_hierarchy.CreateNode();
            EntityNode* nodeData = m_hierarchy.CreateData<EntityNode>(node);
            nodeData->id = entity;

            auto& transform = m_registry.get<Components::Transform>(entity);
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
            if(!toAddToHierarchy.empty())
            {
                entt::entity e = toAddToHierarchy.front();
                toAddToHierarchy.pop();

                auto* transform = m_registry.try_get<Components::Transform>(e);
                if(transform)
                {
                    Containers::DataTree::Node target = transform->parentNode;
                    if(!target.IsValid())
                    {
                        target = m_hierarchy.GetRoot();
                    }

                    m_hierarchy.SetNodeOwner(transform->worldNode, target);

                    DEBUG_LOG("ADDING ENTITY TO HIERARCHY!", transform->worldNode.GetIndex(), target.GetIndex());
                }
            }

            if(!toRemoveFromHierarchy.empty())
            {
                Containers::DataTree::Node n = toRemoveFromHierarchy.front();
                toRemoveFromHierarchy.pop();

                DEBUG_LOG("REMOVING FROM HIERARCHY NODE", n.GetIndex());
                m_hierarchy.FreeNode(n);
            }

            if(!toDestroyEntity.empty())
            {
                entt::entity e = toDestroyEntity.front();
                toDestroyEntity.pop();

                DEBUG_LOG("DESTROYING ENTITY ID", (ui32)e);
                m_registry.destroy(e);
            }
        }
    } // namespace World
} // namespace CPGFramework
