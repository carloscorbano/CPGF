#pragma once

#include "multithread_types.hpp"
#include "../iengine_module.hpp"

#include <mutex>
#include <condition_variable>
#include <vector>
#include <map>

namespace CPGFramework
{
    class Engine;
    namespace Threading
    {
        class Multithread : public IEngineModule
        {
        public:
            friend class CPGFramework::Engine;
            friend struct WorkDescriptor;

            Multithread(Engine* engine);
            ~Multithread();

             /// @brief Try to allocated a thread and bind it to a exclusive work. It will keep allocated until the bounded function returns false.
            /// @param outThreadID The allocated thread ID (can be used to specify work target thread ID).
            /// @param work The function that will be executed by the allocated thread.
            /// @return True if it successfully allocated a thread, false if it fails.
            BOOL TryBindThreadToWork(THREAD_ID& outThreadID, std::function<BOOL()> work);

            /// @brief Submits a work descriptor (same as using the internal work descriptor 'Submit').
            /// @param descriptor The descriptor that will be submitted into the pipeline.
            void Submit(WorkDescriptor& descriptor);

        private:
            /// @brief Helper function that will setup the number of available cores.
            void __INTERNAL__setupProcessorCount();
            
            /// @brief Helper function to check if the system can create new threads (limitted by the number of available cores).
            /// @return True if the number of created threads is lower than the available cores.
            BOOL __INTERNAL__canCreateNewThreads();
            
            /// @brief Helper function to check if there is any idle thread.
            /// @param outIdleID The idle thread ID.
            /// @return True if there is any, false if not.
            BOOL __INTERNAL__hasIdleThread(THREAD_ID& outIdleID);

            /// @brief Helper function to get an available thread ID (this method already calls the 'hasIdleThread' and 'canCreateNewThreads' methods). Use this instead of the other two.
            /// @param outThreadID The available thread ID.
            /// @return True if there is any available thread, false if not.
            BOOL __INTERNAL__getAvailableThread(THREAD_ID& outThreadID);

            /// @brief Helper method to create new thread.
            /// @return The newly thread ID.
            THREAD_ID __INTERNAL__createNewThread();

            /// @brief Helper method to join all thread when the program exits.
            void __INTERNAL__joinAllThreads();

            /// @brief Helper method to get a string from thread ID.
            /// @param id The id to be converted.
            /// @return The string from thread id.
            STRING ToString(const THREAD_ID& id);

            /// @brief Helper method used to queue pool values into the dispatcher queue.
            void __INTERNAL__dispatchWork(Containers::PoolValue<WorkData> pv);
            
            /// @brief Helper method to process the dispatcher queue and send to the target threads queue.
            void __INTERNAL__processDispatcherQueue();

            /// @brief Method that is binded to the threads. It keeps beeing executed until the program exits.
            void __INTERNAL__threadWork();

            /// @brief Helper method that makes the created threads 'sleeps' until there is any work to do.
            void __INTERNAL__threadWaitIdle(const THREAD_ID& id);

            /// @brief Helper method that executes the bounded work function.
            void __INTERNAL__processBoundedWork(const THREAD_ID& id);

            /// @brief Helper method to execute the queue that is specific to the thread.
            void __INTERNAL__processQueue(const THREAD_ID& id);

            /// @brief The function that manages to watch and manage the threads.
            BOOL __INTERNAL__watcherWork();

            /// @brief Helper function to wake up an idle worker that has work to do.
            void __INTERNAL__notifyIdleWorker();

            /// @brief Helper function to be used in the Object Pool for construction of the object.
            WorkData* __INTERNAL__createWorkData();

            /// @brief Helper function to be used in the Object Pool for reseting the data.
            void __INTERNAL__resetWorkData(WorkData& wd);

        private:
            void Initialize() override;
            void Update() override;
            void FixedUpdate() override;
            void LateUpdate() override;
            void Cleanup() override;

        private:
            ui8 m_processorCount;
            std::mutex m_mutex;

            std::vector<std::thread> m_threads;
            std::map<THREAD_ID, ThreadState> m_stateMap;
            std::map<THREAD_ID, std::function<BOOL()>> m_boundedWork;
            std::map<THREAD_ID, std::queue<Containers::PoolValue<WorkData>>> m_threadWorkQueue;
            
            std::queue<Containers::PoolValue<WorkData>> m_dispatcherQueue;

            Containers::ObjectPool<WorkData> m_workDataPool;
            THREAD_ID m_watcherID;
        };
    }
}