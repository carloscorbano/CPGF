#pragma once

#include "../definitions/typedefs.hpp"
#include "../definitions/dll.hpp"
#include <vector>
#include <functional>

namespace CPGFramework 
{
    namespace Containers 
    {
        /// @brief Generic type of list, this list will keep cycling and it'll not increase the list length.
        ///         It keeps looping and inserting the data within the already created nodes.
        /// @tparam T 
        template<typename T>
        class CPGF_API GenericCyclicList 
        {
        public:
            /// @brief Default constructor. This class must be initialized with the total length of the container.
            /// @param length The total length of the container.
            GenericCyclicList(const ui32& length)
                : m_capacity(length), m_count(0), m_head(0), m_tail(0) 
            {
                for(ui32 i = 0; i < length; ++i) 
                {
                    __INTERNAL__CreateElement();
                }
            }

            ~GenericCyclicList() 
            {
                __INTERNAL__Destroy();
            }

            /// @brief Insert the data into the container, It'll always insert the data at the tail index.
            /// @param data The data to be inserted.
            /// @return The reference to the inserted data.
            T& Insert(const T& data) 
            {
                if(GetCapacity() == GetCount() && m_head == m_tail) 
                {
                    __INTERNAL__MoveHeadForward();
                }

                ui32 cachedTail = m_tail;
                __INTERNAL__IncreaseCount();
                __INTERNAL__MoveTailForward();
                return __INTERNAL__Insert(cachedTail, data);
            }

            /// @brief This will remove the head element.
            void RemoveHead() 
            {
                __INTERNAL__MoveHeadForward();
                __INTERNAL__DecreaseCount();
            }

            /// @brief This will remove the tail element.
            void RemoveTail() 
            {
                __INTERNAL__MoveTailBackward();
                __INTERNAL__DecreaseCount();
            }

            /// @brief Get the data from the given index.
            /// @param index The index of the element (it'll be relative to the head position, so index 0 is the same as the head position).
            /// @return The data.
            DLL_EXPORT T& Get(const ui32& index) 
            {
                return *m_container[((m_head + index) % GetCapacity())];
            }

            /// @brief View the container elements.
            /// @param op The operation that will be triggered for each element.
            /// @param reverse If the lookup will be reversed (tail to head).
            DLL_EXPORT void View(std::function<BOOL(T& element)> op, const BOOL& reverse = false) 
            {
                if(!op) return;

                for(ui32 i = 0; i < GetCount(); ++i) 
                {
                    if(!reverse) 
                    {
                        ui32 index = ((m_head + i) % GetCapacity());
                        if(!op(Get(index))) 
                        {
                            break;
                        }
                    }
                    else 
                    {
                        i32 result = (m_tail - 1) - i;
                        if(result < 0) 
                        {
                            result += GetCapacity();
                        }

                        if(!op(Get(result))) 
                        {
                            break;
                        }
                    }
                }
            }

            /// @brief Get the capacity of the container (it's the total size, not the elements count.).
            /// @return The total size of the container.
            const ui32 GetCapacity() const 
            {
                return m_capacity;
            }

            /// @brief Get the number of elements in the container.
            /// @return The number of elements in the container.
            const ui32 GetCount() const 
            {
                return m_count;
            }
        private:
            /// @brief Helper method to insert the data in the given index.
            T& __INTERNAL__Insert(const ui32& index, const T& data) 
            {
                __INTERNAL__SetElement(index, data);
                return *m_container[index];
            }

            /// @brief Helper method to set the data inside the container, with the given index.
            /// @param index 
            /// @param data 
            void __INTERNAL__SetElement(const ui32& index, const T& data) 
            {
                *m_container[index] = data;
            }

            /// @brief Helper method to increase the container elements count.
            void __INTERNAL__IncreaseCount() 
            {
                m_count++;
                if(m_count >= m_capacity) 
                {
                    m_count = m_capacity;
                }
            }

            /// @brief Helper method to decrease the container elements count.
            void __INTERNAL__DecreaseCount() 
            {
                m_count--;
            }

            /// @brief Helper method to destroy the container.
            void __INTERNAL__Destroy() 
            {
                for(ui32 i = 0; i < GetCount(); ++i) 
                {
                    __INTERNAL__DeleteElement(i);
                }
            }

            /// @brief Helper method to create an element (used only in the container creation).
            /// @return 
            T& __INTERNAL__CreateElement() 
            {
                ui32 index = m_container.size();
                m_container.push_back(new T());
                return *m_container[index];
            }

            /// @brief Helper method to delete the element at index.
            /// @param index 
            void __INTERNAL__DeleteElement(const ui32& index) 
            {
                if(m_container[index]) 
                {
                    delete m_container[index];
                    m_container[index] = nullptr;
                }
            }

            /// @brief Helper method to move the tail forward.
            void __INTERNAL__MoveTailForward() 
            {
                m_tail = __INTERNAL__GetNextIndex(m_tail);
            }

            /// @brief Helper method to move the tail backwards.
            void __INTERNAL__MoveTailBackward() 
            {
                m_tail = __INTERNAL__GetIndexBefore(m_tail);
            }

            /// @brief Helper method to move the head forward.
            void __INTERNAL__MoveHeadForward() 
            {
                m_head = __INTERNAL__GetNextIndex(m_head);
            }

            /// @brief Helper method to get the next index from the given index.
            const ui32 __INTERNAL__GetNextIndex(const ui32& current) 
            {
                return (current + 1) % GetCapacity();
            }

            /// @brief Helper method to get the index before the current index.
            const ui32 __INTERNAL__GetIndexBefore(const ui32& current) 
            {
                i32 casted = static_cast<i32>(current);
                BOOL result = casted - 1 < 0;
                if(result) 
                {
                    return GetCapacity() - 1;
                } 
                else 
                {
                    return static_cast<ui32>(casted);
                }
            }

        private:
            std::vector<T*> m_container;
            ui32 m_count;
            ui32 m_capacity;

            ui32 m_head;
            ui32 m_tail;
        };
    } //namespace Containers
} // namespace CeosFramework