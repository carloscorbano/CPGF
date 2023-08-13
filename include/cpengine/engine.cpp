#include "engine.hpp"

namespace CPGFramework
{
    Engine::Engine()
    {
        glfwInit();
        glewExperimental = GL_TRUE;
        glewInit();      

        glfwTerminate();  
    }
    Engine::~Engine()
    {}
} // namespace CPGFramework
