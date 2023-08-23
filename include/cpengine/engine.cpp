#include "engine.hpp"
#include "debug/debug.hpp"
#include "modules/graphics/window.hpp"
#include "modules/threading/multithread.hpp"
#include "modules/time/game_time.hpp"
#include "modules/time/timed_action.hpp"
#include "modules/input/input.hpp"
#include "modules/resources/resources.hpp"
#include "modules/world/world.hpp"

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
        auto wnd = GetModules()->window;

        auto threading = GetModules()->multithread;
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
        m_modulesContainer.modules.window = std::make_shared<Graphics::Window>(this);
        m_modulesContainer.list.push_back(m_modulesContainer.modules.window);
        m_modulesContainer.modules.window->BeginListen<Graphics::OnWindowCloseEvent>((void*)this, [&](void* emitter, void* listener, void* data){ OnWindowCloseEventHandler(emitter, listener, data); });
        //------------------------------- End Window module
        //------------------------------- Multithread Module
        m_modulesContainer.modules.multithread = std::make_shared<Threading::Multithread>(this);
        m_modulesContainer.list.push_back(m_modulesContainer.modules.multithread);
        //------------------------------- End Multithread module
        //------------------------------- GameTime Module
        m_modulesContainer.modules.gametime = std::make_shared<Time::GameTime>(this);
        m_modulesContainer.list.push_back(m_modulesContainer.modules.gametime);
        //------------------------------- End GameTime module
        //------------------------------- Input Module
        m_modulesContainer.modules.input = std::make_shared<Input::InputClass>(this);
        m_modulesContainer.list.push_back(m_modulesContainer.modules.input);
        //------------------------------- End Input module
        //------------------------------- Resources Module
        m_modulesContainer.modules.resources = std::make_shared<Resources::ResourcesClass>(this);
        m_modulesContainer.list.push_back(m_modulesContainer.modules.resources);
        //------------------------------- End Resources module
        //------------------------------- World Module
        m_modulesContainer.modules.world = std::make_shared<World::WorldClass>(this);
        m_modulesContainer.list.push_back(m_modulesContainer.modules.world);
        //------------------------------- End World Module

        //Initialize all modules.
        for(auto& module : m_modulesContainer.list)
        {
            module->Initialize();
        }
    }

    void Engine::__INTERNAL__CleanupModules()
    {
        auto rit = m_modulesContainer.list.rbegin();
        for(; rit != m_modulesContainer.list.rend(); ++rit)
        {
            (*rit)->Cleanup();
        }
    }

    void Engine::__INTERNAL__boundGameThreads()
    {
        auto mthread = GetModules()->multithread;
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
        auto wnd = GetModules()->window;
        auto gtime = GetModules()->gametime;

        switch (m_gameLoopState)
        {
            case _INTERNAL_ThreadControlState::SETUP:
            {
                wnd->SetThreadContext(wnd->GetMainContext());
                OnEngineStart();
                m_gameLoopState = _INTERNAL_ThreadControlState::RUNNING;
            }
            break;
            case _INTERNAL_ThreadControlState::RUNNING:
            {                
                wnd->PollEvents();

                for(auto& module : m_modulesContainer.list)
                {
                    module->Update();
                }

                Update();

                if(m_fixedUpdateTimer.Tick(gtime->GetDeltaTimeUnscaled()))
                {
                    for(auto& module : m_modulesContainer.list)
                    {
                        module->FixedUpdate();
                    }

                    FixedUpdate();
                }

                for(auto& module : m_modulesContainer.list)
                {
                    module->LateUpdate();
                }

                LateUpdate();

                if(!wnd->IsFPSLocked() || m_drawTimer.Tick(gtime->GetDeltaTimeUnscaled()))
                {
                    wnd->Draw();
                }
            }
            break;
            case _INTERNAL_ThreadControlState::ON_FRAMEWORK_QUIT:
            {
                OnEngineQuit();
                //wait resources cleanup
                m_resLoopState = _INTERNAL_ThreadControlState::CLEANUP_RESOURCES;
                
                auto threading = GetModules()->multithread;
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
        auto wnd = GetModules()->window;
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
