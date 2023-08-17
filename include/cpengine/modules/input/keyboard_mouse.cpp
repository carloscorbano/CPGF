#include "keyboard_mouse.hpp"
#include "../../debug/debug.hpp"
#include "../../engine.hpp"
#include "../graphics/window.hpp"
#include "input.hpp"

namespace CPGFramework
{
    namespace Input
    {
        KeyboardMouse::KeyboardMouse() : m_hasChanged(false) {}

        KeyboardMouse::~KeyboardMouse() {}

        void KeyboardMouse::Setup(Engine& engine)
        {
            m_window = engine.GetModules()->window->GetMainContext();
            GLFWCall(glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE));
            GLFWCall(glfwSetScrollCallback(m_window, KeyboardMouse::scroll_callback));
        }

        void KeyboardMouse::Update() 
        {
            __INTERNAL__UpdateKeys();
            __INTERNAL__UpdateMouse();
        }

        BOOL KeyboardMouse::HasChanged() 
        {
            BOOL result = m_hasChanged;
            m_hasChanged = false;
            return result; 
        }

        void KeyboardMouse::__INTERNAL__UpdateKeys()
        {
            for(int i = (int)Key::SPACE; i < (int)GLFW_KEY_LAST; ++i) 
            {
                InputData& data = m_data.keyboardKeys[i];
                GLFWCall(BOOL result = glfwGetKey(m_window, i) == GLFW_PRESS ? true : false);

                if(data.current != result) 
                {
                    m_hasChanged = true;
                }

                data.last = data.current;
                data.current = result;
            }
        }

        void KeyboardMouse::__INTERNAL__UpdateMouse()
        {
            for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
            {
                InputData& data = m_data.mouseButtons[i];
                GLFWCall(BOOL result = glfwGetMouseButton(m_window, i) == GLFW_PRESS ? true : false);

                if(data.current != result) 
                {
                    m_hasChanged = true;
                }

                data.last = data.current;
                data.current = result;
            }

            //cursor pos
            m_data.cursorPos[VAL_LAST] = m_data.cursorPos[VAL_CURRENT];
            GLFWCall(glfwGetCursorPos(m_window, (double*)&m_data.cursorPos[VAL_CURRENT].x, (double*)&m_data.cursorPos[VAL_CURRENT].y));
            
            //update scroll
            m_data.scrollVal[VAL_LAST] = m_data.scrollVal[VAL_CURRENT];
            m_data.scrollVal[VAL_CURRENT] = m_data.scrollVal[VAL_NEXT];
            //reset scroll if the value changed
            if( m_data.scrollVal[VAL_NEXT].x > 0.1 || m_data.scrollVal[VAL_NEXT].x < -0.1 ||
                m_data.scrollVal[VAL_NEXT].y > 0.1 || m_data.scrollVal[VAL_NEXT].y < -0.1) 
            {
                m_data.scrollVal[VAL_NEXT] = {0.0f, 0.0f};
                m_hasChanged = true;
            }
        }

        void KeyboardMouse::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
        {
            Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
            auto& kmo = engine->GetModules()->input->GetKeyboarMouseObj();
            kmo.m_data.scrollVal[VAL_NEXT].x = xoffset;
            kmo.m_data.scrollVal[VAL_NEXT].y = yoffset;
        }
    }   
}