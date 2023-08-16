#pragma once

#include "definitions/dll.hpp"
#include "definitions/typedefs.hpp"
#include "modules/iengine_module.hpp"
#include "containers/unordered_ref_type_map.hpp"
#include <memory>
namespace CPGFramework
{
    class CPGF_API Engine
    {
        enum class _INTERNAL_ThreadControlState { SETUP, RUNNING, ON_FRAMEWORK_QUIT, CLEANUP_RESOURCES, FINISHED };
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

        const THREAD_ID GetGameLoopID() const { return m_gameLoopID; }
        const THREAD_ID GetResourcesLoopID() const { return m_resLoopID; }

        inline const BOOL IsRunning() const { return m_isRunning; }

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

        /// @brief Event raised by the window when it's closed.
        void OnWindowCloseEventHandler(void* emitter, void* listener, void* data);

        /// @brief Create engine modules, setup and initialize.
        void __INTERNAL__CreateModules();
        /// @brief Cleanup all engine modules.
        void __INTERNAL__CleanupModules();

        /// @brief Helper method to bound game threads.
        void __INTERNAL__boundGameThreads();
        /// @brief The main game loop work.
        /// @return If the state is ok.
        BOOL __INTERNAL__gameLoop();
        /// @brief The resources loop work.
        /// @return If the state is ok.
        BOOL __INTERNAL__resourcesLoop();
    private:
        Containers::UnorderedRefTypeMap<std::shared_ptr<IEngineModule>> m_modules;
        BOOL m_isRunning;

        THREAD_ID m_gameLoopID;
        _INTERNAL_ThreadControlState m_gameLoopState;
        THREAD_ID m_resLoopID;
        _INTERNAL_ThreadControlState m_resLoopState;
    };
}