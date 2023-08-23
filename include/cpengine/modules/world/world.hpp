#pragma once

#include "../iengine_module.hpp"
#include "entity.hpp"
#include "../../containers/data_tree.hpp"

namespace CPGFramework
{
    namespace World
    {
        class WorldClass : public IEngineModule
        {
        public:
            WorldClass(Engine* engine);
            ~WorldClass();

            void OnTransformComponentAdded(entt::registry& reg, entt::entity entity);
        private:
            entt::registry m_registry;
        };
    } // namespace World
} // namespace CPGFramework