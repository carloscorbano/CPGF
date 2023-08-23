#include "entity.hpp"
#include "world.hpp"

namespace CPGFramework
{
    namespace World
    {
        entt::registry* Entity::world_reg = nullptr;

        Entity::Entity()
        {
            m_entt = world_reg->create();
        }

        Entity::~Entity()
        {
            Destroy();
        }
    } // namespace World
} // namespace CPGFramework
