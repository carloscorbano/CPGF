#include "engine.hpp"
#include "modules/graphics/window.hpp"
#include "debug/debug.hpp"

namespace CPGFramework
{
    Engine::Engine()
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
        DEBUG_LOG("RUNNING ENGINE");
        GetModule<Graphics::Window>()->RunWindowThreadWork();
    }

    void Engine::OnWindowCloseEventHandler(void* emitter, void* listener, void* data)
    {
        DEBUG_LOG("RAISED ON WINDOW CLOSE EVENT");
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
