#pragma once

#include "iengine_module.hpp"
#include <memory>
#include <vector>

namespace CPGFramework
{
    namespace Graphics      { class Window;         }
    namespace Input         { class InputClass;     }
    namespace Threading     { class Multithread;    }
    namespace Time          { class GameTime;       }

    struct EngineModules
    {
        std::shared_ptr<Graphics::Window>       window = nullptr;
        std::shared_ptr<Input::InputClass>      input = nullptr;
        std::shared_ptr<Threading::Multithread> multithread = nullptr;
        std::shared_ptr<Time::GameTime>         gametime = nullptr;
    };

    struct ModuleContainer
    {
        EngineModules modules;
        std::vector<std::shared_ptr<IEngineModule>> list;
    };
} // namespace CPGFramework
