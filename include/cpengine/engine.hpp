#pragma once

#include "definitions/dll.hpp"
#include "modules/iengine_module.hpp"
#include "containers/unordered_ref_type_map.hpp"
#include <memory>
namespace CPGFramework
{
    class CPENGINE_API Engine
    {
    public:
        template<typename T>
        std::shared_ptr<T> GetModule()
        {
            auto iterator = m_modules.find(typeid(T));
            if(iterator == m_modules.end())
            {
                return nullptr;
            }
            
            return std::static_pointer_cast<T>(m_modules[typeid(T)]);
        }

        void Run();

    protected:
        Engine();
        ~Engine();

    private:
        template<typename T, typename... TArgs>
        std::shared_ptr<IEngineModule> AddModule(TArgs... args)
        {
            auto iterator = m_modules.find(typeid(T));
            if(iterator == m_modules.end())
            {
                m_modules[typeid(T)] = std::make_shared<T>(args...);
            }

            return m_modules[typeid(T)];
        }

        void OnWindowCloseEventHandler();
    private:
        Containers::UnorderedRefTypeMap<std::shared_ptr<IEngineModule>> m_modules;
    };
}