#include "window.hpp"
#include "glfw.hpp"
#include <stdexcept>

#include <iostream>

namespace CPGFramework
{
    namespace Graphics
    {
        Window::Window(Engine* engine)
            : IEngineModule(engine)
        {
            printf("CREATED WINDOW OBJ\n");
        }

        Window::~Window()
        {
            printf("DESTROYED WINDOW OBJ\n");
            GLFWCall(glfwTerminate());
        }

        void Window::SetThreadContext(GLFWwindow* ctx)
        {
            GLFWCall(glfwMakeContextCurrent(ctx));

            if(ctx == nullptr || ctx == NULL) return;

            glewExperimental = GL_TRUE;
            GLenum result = glewInit();
            if (result != GLEW_OK)
            {
                throw std::runtime_error((const char*)glewGetErrorString(result));
            }
        }

        void Window::RunWindowThreadWork()
        {
            while(true)
            {
                if(glfwWindowShouldClose(m_winCtx))
                {
                    if(onWindowCloseEvent)
                    {
                        onWindowCloseEvent();
                    }

                    break;
                }

                GLFWCall(glfwWaitEvents());
            }
        }

        void Window::Initialize()
        {
            //initialize glfw
            GLFWCall(glfwInit());

            GLFWCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4));
            GLFWCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5));
            GLFWCall(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE));
            GLFWCall(glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE));
            GLFWCall(glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE));
            GLFWCall(glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE));
            GLFWCall(glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE));
            GLFWCall(glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE));
            GLFWCall(glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API));

            //Create resources context
            GLFWCall(glfwWindowHint(GLFW_DECORATED, GLFW_FALSE));
            GLFWCall(glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE));
            GLFWCall(m_resCtx = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "resources_context", NULL, NULL));

            //setup context
            SetThreadContext(m_resCtx);

            //create main context
            GLFWCall(glfwWindowHint(GLFW_DECORATED, GLFW_TRUE));
            GLFWCall(glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE));
            GLFWCall(m_winCtx = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE, NULL, m_resCtx));

            //make context
            SetThreadContext(m_winCtx);
        
            //window setup
            GLFWCall(glfwSetWindowSizeLimits(m_winCtx, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE));
            GLFWCall(glfwSwapInterval(1)); //TODO: change this to 0

            //set window pointer
            GLFWCall(glfwSetWindowUserPointer(m_winCtx, &GetEngineRef()));
            //set null context on main thread.
            SetThreadContext(NULL);
        }

        void Window::Update()
        {}

        void Window::FixedUpdate()
        {}

        void Window::LateUpdate()
        {}
    } // namespace Graphics
} // namespace CPGFramework