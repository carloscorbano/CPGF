#pragma once

#include "../../definitions/typedefs.hpp"
#include "../../containers/object_pool.hpp"
#include <functional>
#include <queue>

#define MINIMAL_AMOUNT_OF_CORES 4

namespace CPGFramework
{
    namespace Threading
    {
         /// @brief The thread state to trace and control each thread worker.
        enum class ThreadState { IDLE, BUSY, BOUNDED };
        /// @brief The work state, this control the flow of the working queue.
        enum class WorkState { NONE, PROCESSING, COMPLETED_OK, COMPLETED_ERROR };
        /// @brief Operation Descriptor, this is a container for all the operation queue data.
        struct OperationData 
        {
            OperationData() {}
            virtual ~OperationData() {}
        };

        typedef std::function<BOOL(OperationData*)> OP_FUNC;
        struct OperationDesc 
        {
            THREAD_ID target;
            OP_FUNC operation;
        };

        typedef std::queue<OperationDesc> OP_DESC_QUEUE;
        typedef std::function<void(OperationData* data, const BOOL& success)> CALLBACK_FUNC;
        /// @brief Work data, this is a container to hold all information of the work to be processed.
        struct WorkData 
        {
            OperationData* opData;
            OP_DESC_QUEUE operations;
            THREAD_ID callbackTarget;
            CALLBACK_FUNC callback;
            WorkState state;
        };

        // forward declaration
        class Multithread;
        /// @brief A helper struct to create work.
        struct WorkDescriptor 
        {
            friend class Multithread;

            WorkDescriptor(Multithread* multithreadObj);
            ~WorkDescriptor();

            void* operator new(size_t size) = delete;
            void operator delete(void* obj) = delete;

            /// @brief Bind the given struct/class data to the descriptor.
            /// @tparam T The struct/class that will hold the data to be processed.
            /// @tparam ...TArgs Construction arguments type.
            /// @param ...args Construction arguments values.
            /// @return The pointer to the allocated data.
            template<typename T, typename... TArgs>
            T* BindData(TArgs... args) 
            {
                if(m_pv.Data()->opData == nullptr) 
                {
                    m_pv.Data()->opData = new T(args...);
                }

                return reinterpret_cast<T*>(m_pv.Data()->opData);
            }

            /// @brief Helper method to bind a callback (it can be binded only once). If you try to bind another, the last one will be rewrited.
            /// @param target The thread target ID (NULL_THREAD_ID to be processed on any thread that is not bounded).
            /// @param callback The callback function.
            /// @return this descriptor for chaining methods.
            WorkDescriptor* BindCallback(THREAD_ID target, CALLBACK_FUNC callback);

            /// @brief Helper method to bind operations, it can be chained and it'll follow the sequence operation (operation queue).
            /// @param target The thread target ID (NULL_THREAD_ID to be processed on any thread that is not bounded).
            /// @param operation The operation to be processed, it returns a BOOL (if returns false, the operation will stop and callback will be called, check callback 'success' for this result).
            /// @return this descriptor for chaining operations.
            WorkDescriptor* BindOperation(THREAD_ID target, OP_FUNC operation);

            /// @brief Submit this descriptor into the process pipeline.
            void Submit();

        private:
            Multithread* m_multithread;
            BOOL m_wasSubmitted; //internal value to check if it was submitted, if not, the pipeline will clean the allocated resources.
            Containers::PoolValue<WorkData> m_pv; //the pool value from this descriptor.
        };
    }    
} // namespace CPGFramework
