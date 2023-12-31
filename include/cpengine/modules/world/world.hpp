#pragma once

#include "../../containers/data_tree.hpp"
#include "../iengine_module.hpp"
#include "entt.hpp"
#include <queue>

namespace CPGFramework
{
    namespace World
    {
        class WorldClass : public IEngineModule
        {
        public:
            friend class Engine;
            WorldClass(Engine* engine);
            ~WorldClass();

            inline entt::registry& GetRegistry()        { return m_registry;    }
            inline Containers::DataTree& GetHierarchy() { return m_hierarchy;   }

        private:
            void Update();
            void OnTransformComponentAdded(entt::registry& reg, entt::entity entity);
            void OnTransformComponentDestroyed(entt::registry& reg, entt::entity entity);
            entt::entity OnConstructEntityRequestedCallback();
            void OnDestroyEntityRequestedCallback(entt::entity entity);
            void __INTERNAL__SetupHierarchyRoot();
            void __INTERNAL__BindCallbacks();
        private:
            entt::registry m_registry;
            Containers::DataTree m_hierarchy;

            std::queue<entt::entity> toAddToHierarchy;
            std::queue<Containers::DataTree::Node> toRemoveFromHierarchy;
            std::queue<entt::entity> toDestroyEntity;
        };
    } // namespace World
} // namespace CPGFramework
