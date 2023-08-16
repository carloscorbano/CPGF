#include "multithread_types.hpp"
#include "multithread.hpp"

namespace CPGFramework
{
    namespace Threading
    {
        WorkDescriptor::WorkDescriptor(Multithread* multithreadObj) 
            : m_wasSubmitted(false), m_multithread(multithreadObj),
              m_pv(multithreadObj->m_workDataPool.GetAvailable())
        {}

        WorkDescriptor::~WorkDescriptor() 
        {
            if(!m_wasSubmitted) 
            {
                m_pv.Release();
            }
        }

        WorkDescriptor* WorkDescriptor::BindCallback(std::thread::id target, std::function<void(OperationData* data, const BOOL& success)> callback) 
        {
            if(m_wasSubmitted) return nullptr;

            WorkData* data = m_pv.Data();
            data->callbackTarget = target;
            data->callback = callback;
            return this;
        }

        WorkDescriptor* WorkDescriptor::BindOperation(std::thread::id target, std::function<BOOL(OperationData* data)> operation)
        {
            if(m_wasSubmitted) return nullptr;

            WorkData* data = m_pv.Data();
            data->operations.push({ target, operation });
            return this;
        }

        void WorkDescriptor::Submit() 
        {
            if(m_wasSubmitted) return;

            m_multithread->Submit(*this);
        }
    }
}