#pragma once

#include "entt.hpp"
#include "../../definitions/dll.hpp"

namespace CPGFramework
{
    namespace World
    {
        class WorldClass;

        class Entity
        {
        public:
            friend class WorldClass;
            Entity();
            ~Entity();

            template<typename T, typename... TArgs>
            T& AddComponent(TArgs... args)
            {
                return world_reg->get_or_emplace<T>(m_entt, args...);
            }

            template<typename T>
            T* GetComponent()
            {
                return &world_reg->try_get<T>(m_entt);
            }

            template<typename T>
            void RemoveComponent()
            {
                if(GetComponent<T>())
                {
                    world_reg->erase<T>(m_entt);
                }
            }

            void Destroy()
            {
                world_reg->destroy(m_entt);
            }
        private:
            entt::entity m_entt;
            CPGF_API static entt::registry* world_reg;
        };
    } // namespace World
} // namespace CPGFramework
