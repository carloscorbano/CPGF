#include "engine.hpp"
#include "modules/graphics/window.hpp"
#include <iostream>

namespace CPGFramework
{
    Engine::Engine()
    {
        printf("ENGINE CREATION AND INITIALIZATION OF ENGINE MODULES\n");
        AddModule<Graphics::Window>(this);
        GetModule<Graphics::Window>()->onWindowCloseEvent = std::bind(&Engine::OnWindowCloseEventHandler, this);
    }

    Engine::~Engine()
    {
        printf("DESTROYED ENGINE OBJ\n");
    }

    void Engine::Run()
    {
        printf("RUNNING ENGINE\n");
        auto wnd = GetModule<Graphics::Window>();
        wnd->Initialize();
        wnd->RunWindowThreadWork();
    }

    void Engine::OnWindowCloseEventHandler()
    {
        printf("RAISED ON WINDOW CLOSE EVENT\n");
    }
} // namespace CPGFramework
