#pragma once

#include "entt.hpp"
#include "../../definitions/dll.hpp"
#include <functional>

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
            Entity(entt::entity entity);
            ~Entity();

            template<typename T, typename... TArgs>
            T* AddComponent(TArgs... args)
            {
                return &worldRegistry->get_or_emplace<T>(m_entt, args...);
            }

            template<typename T>
            T* GetComponent()
            {
                return worldRegistry->try_get<T>(m_entt);
            }

            template<typename T>
            void RemoveComponent()
            {
                if(GetComponent<T>())
                {
                    worldRegistry->erase<T>(m_entt);
                }
            }

            void Destroy()
            {
                if(onDestroyRequest)
                {
                    onDestroyRequest(m_entt);
                }
            }
        private:
            entt::entity m_entt;
            CPGF_API static entt::registry* worldRegistry;
            static std::function<entt::entity()> onConstructEntityRequest;
            static std::function<void(entt::entity entity)> onDestroyRequest;
        };
    } // namespace World
} // namespace CPGFramework
