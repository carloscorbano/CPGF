#include "engine.hpp"
#include "debug/debug.hpp"
#include "modules/graphics/window.hpp"
#include "modules/threading/multithread.hpp"
#include "modules/time/game_time.hpp"
#include "modules/time/timed_action.hpp"

namespace CPGFramework
{
    Engine::Engine()
        : m_isRunning(true), m_gameLoopState(_INTERNAL_ThreadControlState::SETUP),
          m_resLoopState(_INTERNAL_ThreadControlState::SETUP),
          m_fixedUpdateTimer(DEFAULT_FIXED_UPDATE_TIME),
          m_drawTimer(DEFAULT_DRAW_TIMER)
    {
        DEBUG_LOG("Created Engine obj. Loading Engine Modules...");
        __INTERNAL__CreateModules();
    }

    Engine::~Engine()
    {
        DEBUG_LOG("DESTROYED ENGINE OBJ");
    }

    void Engine::Run()
    {
        __INTERNAL__boundGameThreads();
        DEBUG_LOG("RUNNING ENGINE");
        auto wnd = GetModule<Graphics::Window>();

        auto threading = GetModule<Threading::Multithread>();
        threading->YieldUntil([&]() { return wnd->WindowWaitEvents(); });
        threading->YieldUntil([&]() { return m_gameLoopState != _INTERNAL_ThreadControlState::FINISHED; });

        __INTERNAL__CleanupModules();
    }

    void Engine::OnWindowCloseEventHandler(void* emitter, void* listener, void* data)
    {
        DEBUG_LOG("RAISED ON WINDOW CLOSE EVENT");
        m_gameLoopState = _INTERNAL_ThreadControlState::ON_FRAMEWORK_QUIT;
    }

    void Engine::__INTERNAL__CreateModules()
    {
        //------------------------------- Window module
        AddModule<Graphics::Window>(this);
        auto wnd = GetModule<Graphics::Window>();
        wnd->BeginListen<Graphics::OnWindowCloseEvent>((void*)this, [&](void* emitter, void* listener, void* data){ OnWindowCloseEventHandler(emitter, listener, data); });
        //------------------------------- End Window module
        //------------------------------- Multithread Module
        AddModule<Threading::Multithread>(this);
        //------------------------------- End Multithread module
        AddModule<Time::GameTime>(this);

        //Initialize all modules.
        for(auto& module : m_modules)
        {
            module.second->Initialize();
        }
    }

    void Engine::__INTERNAL__CleanupModules()
    {
        for(auto& module : m_modules)
        {
            module.second->Cleanup();
        }
    }

    void Engine::__INTERNAL__boundGameThreads()
    {
        auto mthread = GetModule<Threading::Multithread>();
        if(!mthread->TryBindThreadToWork(m_gameLoopID, std::bind(&Engine::__INTERNAL__gameLoop, this))) 
        {
            throw std::runtime_error("Failed to create game loop thread");
        }

        if(!mthread->TryBindThreadToWork(m_resLoopID, std::bind(&Engine::__INTERNAL__resourcesLoop, this))) 
        {
            throw std::runtime_error("Failed to create resources loop thread");
        }
    }

    BOOL Engine::__INTERNAL__gameLoop()
    {
        auto wnd = GetModule<Graphics::Window>();
        auto gtime = GetModule<Time::GameTime>();

        switch (m_gameLoopState)
        {
            case _INTERNAL_ThreadControlState::SETUP:
            {
                wnd->SetThreadContext(wnd->GetMainContext());
                m_gameLoopState = _INTERNAL_ThreadControlState::RUNNING;
            }
            break;
            case _INTERNAL_ThreadControlState::RUNNING:
            {                
                wnd->PollEvents();

                for(auto module : m_modules)
                {
                    module.second->Update();
                }

                Update();

                if(m_fixedUpdateTimer.Tick(gtime->GetDeltaTimeUnscaled()))
                {
                    for(auto module : m_modules)
                    {
                        module.second->FixedUpdate();
                    }

                    FixedUpdate();
                }

                for(auto module : m_modules)
                {
                    module.second->LateUpdate();
                }

                LateUpdate();

                if(m_drawTimer.Tick(gtime->GetDeltaTimeUnscaled()))
                {
                    wnd->Draw();
                }
            }
            break;
            case _INTERNAL_ThreadControlState::ON_FRAMEWORK_QUIT:
            {
                //wait resources cleanup
                m_resLoopState = _INTERNAL_ThreadControlState::CLEANUP_RESOURCES;
                
                auto threading = GetModule<Threading::Multithread>();
                threading->YieldUntil([&]() { return m_resLoopState != _INTERNAL_ThreadControlState::FINISHED; });
                
                m_isRunning = false;
                m_gameLoopState = _INTERNAL_ThreadControlState::FINISHED;
            }
            break;
            default: break;
        }

        return true;
    }

    BOOL Engine::__INTERNAL__resourcesLoop()
    {
        auto wnd = GetModule<Graphics::Window>();
        switch (m_resLoopState)
        {
            case _INTERNAL_ThreadControlState::SETUP:
            {
                wnd->SetThreadContext(wnd->GetResourcesContext());
                m_resLoopState = _INTERNAL_ThreadControlState::RUNNING;
            }
            break;
            case _INTERNAL_ThreadControlState::RUNNING:
            {
                wnd->PollEvents();
            }
            break;
            case _INTERNAL_ThreadControlState::CLEANUP_RESOURCES:
            {
                //TODO: implement resources module and cleanup it.
                m_resLoopState = _INTERNAL_ThreadControlState::FINISHED;
            }
            break;
            default: break;
        }

        return true;
    }
} // namespace CPGFramework
