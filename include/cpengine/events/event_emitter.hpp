#pragma once

#include <map>
#include <functional>
#include "../definitions/dll.hpp"
#include "../containers/unordered_ref_type_map.hpp"

namespace CPGFramework 
{
    namespace Events 
    {
        typedef std::map<void*, std::function<void(void* emitter, void* listener, void* data)>> EventData;
        class EventEmitter 
        {
        public:
            EventEmitter() {}
            ~EventEmitter() 
            {
                m_boundedEvents.clear();
            }
            
            /// @brief Start to listen for the given event.
            /// @tparam T The data container type to be bounded.
            /// @param listener The listener object
            /// @param callback The callback function that will be called when the event is triggered.
            ///                 It returns the emitter object, the listener object and the data object.
            template<typename T>
            inline void BeginListen(void* listener, std::function<void(void* emitter, void* listener, void* data)> callback) 
            {
                m_boundedEvents[typeid(T)][listener] = callback;
            }

            /// @brief Stop listening the given event type.
            /// @tparam T The type that was bounded before
            /// @param listener The listener to be removed.
            template<typename T>
            inline void StopListening(void* listener) 
            {
                EventData& ed = m_boundedEvents[typeid(T)];
                auto it = ed.find(listener);
                if(it != ed.end())
                {
                    ed.erase(it);
                }
            }

        protected:
            /// @brief Triggers the event.
            /// @tparam T The container type to trigger all the listeners.
            /// @param data The data to be sended to all callbacks.
            template<typename T>
            inline void TriggerEvent(T& data) 
            {
                for(auto& listeners : m_boundedEvents[typeid(T)]) 
                {
                    if(listeners.second) 
                    {
                        listeners.second(this, listeners.first, &data);
                    }
                }
            }
        private:
            Containers::UnorderedRefTypeMap<EventData> m_boundedEvents;
        };
    }
}