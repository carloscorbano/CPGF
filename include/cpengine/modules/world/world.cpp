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
            Entity::world_reg = &m_registry;
            m_registry.on_construct<Components::Transform>().connect<&WorldClass::OnTransformComponentAdded>(this);
        }

        WorldClass::~WorldClass()
        {}

        void WorldClass::OnTransformComponentAdded(entt::registry& reg, entt::entity entity)
        {
            DEBUG_LOG("ADDED TRANSFORM COMPONENT");
        }
    } // namespace World
} // namespace CPGFramework
