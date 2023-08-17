#include "multithread.hpp"
#include "../../debug/debug.hpp"
#include "../../engine.hpp"

namespace CPGFramework
{
    namespace Threading
    {
        Multithread::Multithread(Engine* engine)
            : IEngineModule(engine), m_workDataPool([&] { return __INTERNAL__createWorkData(); }, [&] (WorkData& data) { __INTERNAL__resetWorkData(data); })
        {}
        
        Multithread::~Multithread()
        {}

        void Multithread::YieldUntil(std::function<BOOL()> condition)
        {
            if(!condition) return;
            
            while(condition())
            {
                std::this_thread::yield();
            }
        }

        void Multithread::Initialize()
        {
            DEBUG_LOG("Multithread manager started and running on thread ID [", ToString(std::this_thread::get_id()), "]");
            __INTERNAL__setupProcessorCount();

            //bind the watcher thread worker (this worker will be responsible to manage async threads)
            if(!TryBindThreadToWork(m_watcherID, std::bind(&Multithread::__INTERNAL__watcherWork, this))) 
            {
                throw std::runtime_error("Failed to bind watcher thread worker");
            }
        }

        void Multithread::Cleanup()
        {
            __INTERNAL__joinAllThreads();
        }

        BOOL Multithread::TryBindThreadToWork(THREAD_ID& outThreadID, std::function<BOOL()> work) 
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if(__INTERNAL__getAvailableThread(outThreadID)) 
            {
                m_boundedWork[outThreadID] = work;
                m_stateMap[outThreadID] = ThreadState::BOUNDED;
                DEBUG_SUCCESS("Successfully bounded thread ID [", ToString(outThreadID), "] to work.");
                return true;
            }

            DEBUG_ERROR("Failed to bind thread work!");
            outThreadID = NULL_THREAD_ID;
            return false;
        }

        void Multithread::Submit(WorkDescriptor& descriptor) 
        {
            descriptor.m_wasSubmitted = true;
            descriptor.m_pv.Data()->state = WorkState::PROCESSING;
            __INTERNAL__dispatchWork(descriptor.m_pv);
        }

        void Multithread::__INTERNAL__setupProcessorCount() 
        {
            m_processorCount = (ui8)std::thread::hardware_concurrency();
            DEBUG_LOG("Number of processors: ", std::to_string(m_processorCount));
        }

        BOOL Multithread::__INTERNAL__canCreateNewThreads() 
        {
            return m_threads.size() < m_processorCount;
        }

        BOOL Multithread::__INTERNAL__hasIdleThread(THREAD_ID& outIdleID) 
        {
            for(auto& state : m_stateMap) 
            {
                if(state.second == ThreadState::IDLE) 
                {
                    outIdleID = state.first;
                    return true;
                }
            }

            outIdleID = NULL_THREAD_ID;
            return false;
        }

        BOOL Multithread::__INTERNAL__getAvailableThread(THREAD_ID& outThreadID) 
        {
            if(__INTERNAL__hasIdleThread(outThreadID)) 
            {
                return true;
            }
            else if(__INTERNAL__canCreateNewThreads()) 
            {
                outThreadID = __INTERNAL__createNewThread();
                return true;
            } 
            else 
            {
                outThreadID = NULL_THREAD_ID;
                return false;
            }
        }

        THREAD_ID Multithread::__INTERNAL__createNewThread() 
        {
            m_threads.push_back(std::thread([&] { __INTERNAL__threadWork(); }));
            THREAD_ID id = m_threads[m_threads.size() - 1].get_id();
            m_stateMap[id] = ThreadState::IDLE;
            m_threadWorkQueue[id] = std::queue<Containers::PoolValue<WorkData>>();

            DEBUG_LOG("Created Thread Worker with ID [", ToString(id), "]");
            return id;
        }

        void Multithread::__INTERNAL__joinAllThreads() 
        {
            for(auto& thread : m_threads) 
            {
                if(thread.joinable()) 
                {
                    thread.join();
                }
            }
        }

        STRING Multithread::ToString(const THREAD_ID& id) 
        {
            std::stringstream ss;
            ss << id;
            return ss.str();
        }

        void Multithread::__INTERNAL__dispatchWork(Containers::PoolValue<WorkData> pv) 
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_dispatcherQueue.push(pv);
        }

        void Multithread::__INTERNAL__processDispatcherQueue() 
        {
            if(m_dispatcherQueue.empty()) return;

            //get front dispatcher value
            Containers::PoolValue<WorkData> pvWd = m_dispatcherQueue.front();
            WorkData* wd = pvWd.Data();

            //get next target thread.
            THREAD_ID target;
            switch (wd->state)
            {
                case WorkState::PROCESSING:
                    target = wd->operations.front().target;
                    break;
                case WorkState::COMPLETED_ERROR:
                case WorkState::COMPLETED_OK:
                    target = wd->callbackTarget;
                    break;
                default:
                    DEBUG_ERROR("SOMETHING WRONG HAPPENNED WHILE DISPATCHING FROM DISPATCHER QUEUE.");
                    {
                        std::scoped_lock<std::mutex> lock(m_mutex);
                        m_dispatcherQueue.pop();
                    }
                    pvWd.Release();
                    return;
            }

            //check if thread already exists.
            BOOL success = false;
            for(auto& t : m_threads) 
            {
                if(t.get_id() == target) 
                {
                    success = true;
                    break;
                }
            }

            //if exists the thread
            if(success && target != NULL_THREAD_ID) 
            {
                std::scoped_lock<std::mutex> lock(m_mutex);
                m_dispatcherQueue.pop();
                m_threadWorkQueue[target].push(pvWd);
                return;
            }

            //if not exists, then check if can create
            THREAD_ID tmp;
            if(__INTERNAL__getAvailableThread(tmp)) 
            {
                //dispatch to the thread queue
                std::scoped_lock<std::mutex> lock(m_mutex);
                m_dispatcherQueue.pop();
                m_threadWorkQueue[tmp].push(pvWd);
                return;
            }
            //if it reach here, then there isn't any available thread or it can't create new ones. So it'll continue in the queue to be processed later.
        }

        void Multithread::__INTERNAL__threadWork() 
        {
            const THREAD_ID thisID = std::this_thread::get_id();
            DEBUG_LOG("Started Thread Worker with ID [", ToString(thisID), "]");

            while(GetEngineRef().IsRunning()) 
            {
                //wait activation
                __INTERNAL__threadWaitIdle(thisID);
                if(!GetEngineRef().IsRunning()) break;
                //process worker queue
                __INTERNAL__processQueue(thisID);
                //if bounded, then process bounded work.
                switch (m_stateMap[thisID])
                {
                    case ThreadState::BOUNDED:
                        {
                            __INTERNAL__processBoundedWork(thisID); 
                            //since bounded work will not wait in thread wait idle, then yield here.
                            std::this_thread::yield();
                        }   break;
                    case ThreadState::BUSY: 
                        {
                            //check if all the work is done, then go back to idle.
                            if(m_threadWorkQueue[thisID].empty()) {
                                m_stateMap[thisID] = ThreadState::IDLE;
                            }
                        }   break;
                    default: break;
                }
            }

            DEBUG_LOG("Stopped Thread Worker with ID [", ToString(thisID), "]");
        }

        void Multithread::__INTERNAL__threadWaitIdle(const THREAD_ID& id) 
        {
            while(m_stateMap[id] == ThreadState::IDLE && GetEngineRef().IsRunning()) 
            {
                std::this_thread::yield();
            }
        }

        void Multithread::__INTERNAL__processBoundedWork(const THREAD_ID& id) 
        {
            //run bounded work, if it returns false, then release the bounded worker.
            if(!m_boundedWork[id]()) 
            {
                for(std::map<THREAD_ID, std::function<BOOL()>>::iterator it = m_boundedWork.begin(); it != m_boundedWork.end(); ++it) 
                {
                    if(it->first == id) 
                    {
                        m_boundedWork.erase(it);
                        break;
                    }
                }

                m_stateMap[id] = ThreadState::IDLE;
                DEBUG_LOG("Thread ID [", ToString(id), "] was released from bounded work.");
            }
        }

        void Multithread::__INTERNAL__processQueue(const THREAD_ID& id) 
        {
            //if there is none work to process, then return.
            if(m_threadWorkQueue[id].empty()) return;

            //get the next work
            Containers::PoolValue<WorkData> work = m_threadWorkQueue[id].front();
            
            {
                std::scoped_lock<std::mutex> lock(m_mutex);
                m_threadWorkQueue[id].pop();
            }

            //process work
            WorkData* wd = work.Data();
            //cache the success value
            BOOL success = true;
            switch (wd->state)
            {
                case WorkState::PROCESSING:
                {
                    //cache operation
                    OperationDesc desc = wd->operations.front();
                    wd->operations.pop();
                    //run the operation
                    if(desc.operation) {
                        success = desc.operation(wd->opData);
                    }
                    //if the operation was successfully and it was the last one, the set to completed ok
                    if(success && wd->operations.size() == 0) {
                        wd->state = WorkState::COMPLETED_OK;
                    } else if(!success) { //if was not successfully, then set to completed error
                        wd->state = WorkState::COMPLETED_ERROR;
                    }

                    //dispatch the work to run in the next thread id.
                    __INTERNAL__dispatchWork(work);
                }   break;
                case WorkState::COMPLETED_ERROR:
                    success = false;                    
                case WorkState::COMPLETED_OK: //run the callback and then release the pool value.
                    if(wd->callback) wd->callback(wd->opData, success);
                    work.Release();
                    break;
                default:
                    DEBUG_ERROR("SOMETHING WRONG HAPPENNED WHILE PROCESSING WORK DATA. RELEASING DATA...");
                    work.Release();
                    break;
            }
        }

        BOOL Multithread::__INTERNAL__watcherWork() 
        {
            //process dispatcher queue
            __INTERNAL__processDispatcherQueue();
            //check the queue and wake up the thread that has work to do.
            __INTERNAL__notifyIdleWorker();

            return true;
        }

        void Multithread::__INTERNAL__notifyIdleWorker() 
        {
            for(auto& work : m_threadWorkQueue) 
            {
                THREAD_ID curID = work.first;
                if(!work.second.empty() && m_stateMap[curID] == ThreadState::IDLE) 
                {
                    m_stateMap[curID] = ThreadState::BUSY;
                }
            }
        }

        WorkData* Multithread::__INTERNAL__createWorkData() 
        {
            return new WorkData();
        }

        void Multithread::__INTERNAL__resetWorkData(WorkData& wd) 
        {
            if(wd.opData) 
            {
                delete wd.opData;
                wd.opData = nullptr;
            }

            while(wd.operations.size() > 0) 
            {
                wd.operations.pop();
            }

            wd.state = WorkState::NONE;
            wd.callback = nullptr;
            wd.callbackTarget = NULL_THREAD_ID;
        }
    }
} // namespace CPGFramework
