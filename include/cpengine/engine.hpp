#pragma once

#include "definitions/typedefs.hpp"
#include "modules/module_container.hpp"
#include "modules/time/timed_action.hpp"

#include <memory>
namespace CPGFramework
{
    class Engine
    {
        enum class _INTERNAL_ThreadControlState { SETUP, RUNNING, ON_FRAMEWORK_QUIT, CLEANUP_RESOURCES, FINISHED };
    public:
        inline EngineModules* GetModules() { return &m_modulesContainer.modules; }

        void Run();

        const THREAD_ID GetGameLoopID() const { return m_gameLoopID; }
        const THREAD_ID GetResourcesLoopID() const { return m_resLoopID; }

        inline const BOOL IsRunning() const { return m_isRunning; }

    protected:
        Engine();
        ~Engine();

        virtual void OnEngineStart() = 0;
        virtual void OnEngineQuit() = 0;
        virtual void Update() = 0;
        virtual void FixedUpdate() = 0;
        virtual void LateUpdate() = 0;
        virtual void Draw() = 0;

    private:
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
        ModuleContainer m_modulesContainer;
        BOOL m_isRunning;

        THREAD_ID m_gameLoopID;
        THREAD_ID m_resLoopID;
        _INTERNAL_ThreadControlState m_gameLoopState;
        _INTERNAL_ThreadControlState m_resLoopState;

        Time::TimedAction m_fixedUpdateTimer;
        Time::TimedAction m_drawTimer;
    };
}