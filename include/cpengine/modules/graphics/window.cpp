#include "window.hpp"
#include "glfw.hpp"
#include <stdexcept>
#include "../../debug/debug.hpp"

namespace CPGFramework
{
    namespace Graphics
    {
        Window::Window(Engine* engine)
            : IEngineModule(engine)
        {
            DEBUG_LOG("CREATED WINDOW OBJ");
        }

        Window::~Window()
        {}

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

        bool Window::WindowWaitEvents()
        {
            if(glfwWindowShouldClose(m_winCtx))
            {
                OnWindowCloseEvent owce{};
                TriggerEvent<OnWindowCloseEvent>(owce);
                return false;
            }

            GLFWCall(glfwWaitEvents());
            return true;
        }

        void PollEvents()
        {
            GLFWCall(glfwPollEvents());
        }

        void Window::Draw()
        {
            GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            //TODO: render draw
            
            GLFWCall(glfwSwapBuffers(m_winCtx));
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

            //centralize the window in the monitor.
            //TODO: change the get primary monitor to a saved one.
            __INTERNAL__centralizeWindow(glfwGetPrimaryMonitor());
        }

        void Window::Update()
        {}

        void Window::FixedUpdate()
        {}

        void Window::LateUpdate()
        {}

        void Window::Cleanup()
        {
            GLFWCall(glfwTerminate());
        }

        void Window::__INTERNAL__centralizeWindow(GLFWmonitor* monitor)
        {
            GLFWCall(const GLFWvidmode* vm = glfwGetVideoMode(monitor));
            int w, h;
            GLFWCall(glfwGetWindowSize(m_winCtx, &w, &h));

            int whw, whh;
            whw = (int)((float)w * 0.5f);
            whh = (int)((float)h * 0.5f);

            int mhw, mhh;
            mhw = (int)((float)vm->width * 0.5f);
            mhh = (int)((float)vm->height * 0.5f);

            GLFWCall(glfwSetWindowPos(m_winCtx, mhw - whw, mhh - whh));
        }
    } // namespace Graphics
} // namespace CPGFramework