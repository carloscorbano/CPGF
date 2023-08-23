#include "entity.hpp"
#include "world.hpp"
#include "../../debug/debug.hpp"

namespace CPGFramework
{
    namespace World
    {
        entt::registry* Entity::worldRegistry = nullptr;
        std::function<void(entt::entity entity)> Entity::onDestroyRequest = nullptr;
        std::function<entt::entity()> Entity::onConstructEntityRequest = nullptr;

        Entity::Entity()
        {
            if(onConstructEntityRequest)
            {
                m_entt = onConstructEntityRequest();
            }
            else
            {
                DEBUG_WARNING("ON CONSTRUCT ENTITY REQUEST WAS NOT SET, USING DEFAULT CREATION METHOD.");
                m_entt = worldRegistry->create();
            }

            DEBUG_LOG("CREATED ENTITY ID", std::to_string((ui32)m_entt), ".");
        }

        Entity::Entity(entt::entity entity)
        {
            m_entt = entity;
        }

        Entity::~Entity()
        {
            Destroy();
        }
    } // namespace World
} // namespace CPGFramework
