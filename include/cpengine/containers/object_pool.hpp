#pragma once

#include "../definitions/typedefs.hpp"
#include "../definitions/dll.hpp"
#include <vector>
#include <stack>
#include <mutex>
#include <assert.h>
#include <functional>

namespace CPGFramework 
{
    namespace Containers 
    {

        template<typename T>
        class ObjectPool;

        /// @brief A container for the value from the object pool, it contains all the information needed to access the data and helper methods to do it.
        /// @tparam T 
        template<typename T>
        struct PoolValue 
        {
        private:
            PoolValue(const ui32 index, ObjectPool<T>& owner)
                : m_index(index), m_ownerPool(&owner) {}
        public:
            ~PoolValue() {}

            friend class ObjectPool<T>;

            /// @brief Retrive the data of this value pool object.
            /// @return The pointer to the data object.
            T* Data() 
            {
                if(!m_ownerPool) return nullptr;
                return m_ownerPool->Get(*this);
            }

            /// @brief Release this data object from beeing used, it'll turn this address available to another pickup from the pool.
            void Release() 
            {
                if(!m_ownerPool) return;
                m_ownerPool->Release(*this);
            }

        private:
            const ui32 m_index;                 //the index of the data in the pool.
            ObjectPool<T>* m_ownerPool;         //pointer to the owner pool object.
    };

    /// @brief The object pool container.
    /// @tparam T 
    template<typename T>
    class CPGF_API ObjectPool 
    {
        public:
            friend struct PoolValue<T>;

            /// @brief Default constructor.
            /// @param default_constructor The default constructor for the object pool type, it'll be used to create new instances of T.
            /// @param resetFunction The reset function, it'll be called when the object was released and, by default, it'll set the data to it's default class constructor, 
            /// change it to change the behaviour (param T& data is the reference to the data itself, the data that is beeing released).
            ObjectPool(std::function<T*()> default_constructor, std::function<void(T&)> resetFunction) 
                : m_objDefaultConstructor(default_constructor), m_resetFunction(resetFunction) 
            {}

            ~ObjectPool() 
            {
                __INTERNAL__destroyPool();
            }

            /// @brief Get an available object from the pool or create a new one
            /// @return 
            PoolValue<T> GetAvailable() 
            {
                if(__INTERNAL__hasAvailableIndex()) 
                {
                    return __INTERNAL__getAvailablePoolObject();
                } 
                else 
                {
                    return __INTERNAL__createNewPoolObject();
                }
            }

            /// @brief Retrieve the object data from the given PoolValue.
            /// @param pv 
            /// @return 
            T* Get(const PoolValue<T>& pv) 
            {
                return m_objectPool[pv.m_index];
            }

            /// @brief Helper function to create a pool value from the given id.
            /// @param id 
            /// @return 
            PoolValue<T> CreateFromId(const ui32& id) 
            {
                assert(id < m_objectPool.size());
                return PoolValue<T>(id, *this);
            }

            /// @brief Helper function to retrieve the id from the given pool value.
            /// @param pv 
            /// @return 
            const ui32 GetIdFromValue(const PoolValue<T>& pv) 
            {
                return pv.m_index;
            }

            /// @brief Release the given PoolValue and it's data.
            /// @param pv 
            void Release(PoolValue<T>& pv) 
            {
                __INTERNAL__releasePoolValue(pv);
            }

            /// @brief Get the pool size.
            /// @return 
            const ui32 TotalSize() 
            {
                return (ui32)m_objectPool.size();
            }

            /// @brief Get the number of indexes allocated
            /// @return 
            const ui32 UsageCount() 
            {
                return TotalSize() - m_availableIndexes.size();
            }

            /// @brief Get all objects of the pool.
            /// @return 
            std::vector<T*>& GetObjectPool() 
            {
                return m_objectPool;
            }

        private:
            /// @brief Check if there is any available indexes in the pool
            /// @return 
            BOOL __INTERNAL__hasAvailableIndex() 
            {
                return !m_availableIndexes.empty();
            }

            /// @brief Retrieves an available pool object.
            /// @return 
            PoolValue<T> __INTERNAL__getAvailablePoolObject() 
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                ui32 index = m_availableIndexes.top();
                m_availableIndexes.pop();
                return PoolValue<T>(index, *this);
            }

            /// @brief Creates a new pool object
            /// @return 
            PoolValue<T> __INTERNAL__createNewPoolObject() 
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                ui32 index = m_objectPool.size();
                m_objectPool.push_back(m_objDefaultConstructor());

                return PoolValue<T>(index, *this);
            }

            /// @brief Helper method to destroy the pool objects.
            void __INTERNAL__destroyPool() 
            {
                for(size_t i = 0; i < m_objectPool.size(); ++i) 
                {
                    if(m_objectPool[i]) 
                    {
                        delete m_objectPool[i];
                    }
                }
            }

            /// @brief Helper method to release a given PoolValue object
            /// @param pv 
            void __INTERNAL__releasePoolValue(PoolValue<T>& pv) 
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                
                if(m_resetFunction) 
                {
                    m_resetFunction(*m_objectPool[pv.m_index]);
                }

                pv.m_ownerPool = nullptr;
                m_availableIndexes.push(pv.m_index);
            }
        private:
            std::vector<T*> m_objectPool;
            std::stack<ui32> m_availableIndexes;
            std::mutex m_mutex;

            std::function<T*()> m_objDefaultConstructor;
            std::function<void(T&)> m_resetFunction;
        };
    } // namespace Containers
} // namespace CeosFramework