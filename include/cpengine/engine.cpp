#include "engine.hpp"
#include "modules/graphics/window.hpp"
#include <iostream>

namespace CPGFramework
{
    Engine::Engine()
    {
        printf("ENGINE CREATION AND INITIALIZATION OF ENGINE MODULES\n");
        __INTERNAL__CreateModules();
    }

    Engine::~Engine()
    {
        printf("DESTROYED ENGINE OBJ\n");
    }

    void Engine::Run()
    {
        printf("RUNNING ENGINE\n");
        GetModule<Graphics::Window>()->RunWindowThreadWork();
    }

    void Engine::OnWindowCloseEventHandler(void* emitter, void* listener, void* data)
    {
        
        printf("RAISED ON WINDOW CLOSE EVENT\n");
    }

    void Engine::__INTERNAL__CreateModules()
    {
        //------------------------------- Window module
        AddModule<Graphics::Window>(this);
        auto wnd = GetModule<Graphics::Window>();
        wnd->BeginListen<Graphics::OnWindowCloseEvent>((void*)this, [&](void* emitter, void* listener, void* data){ OnWindowCloseEventHandler(emitter, listener, data); });
        wnd->Initialize();
        //------------------------------- End Window module
    }

    void Engine::__INTERNAL__CleanupModules()
    {
        GetModule<Graphics::Window>()->Cleanup();
    }
} // namespace CPGFramework
