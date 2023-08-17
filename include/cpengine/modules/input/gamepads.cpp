#include "gamepads.hpp"
#include "../../debug/debug.hpp"

namespace CPGFramework
{
    namespace Input
    {
        Gamepads::Gamepads() : m_hasChanged(false), m_numConnectedGamepads(0) 
        {
            for(ui8 i = 0; i < NUM_MAX_CONNECTED_GAMEPADS; ++i) 
            { 
                GamepadData& data = m_data[i];
                data.id = i;
                data.connected = false;
                data.invertYAxis[0] = m_data[i].invertYAxis[1] = false;

                data.deadzones[GLFW_GAMEPAD_AXIS_LEFT_X] = 0.25f;
                data.deadzones[GLFW_GAMEPAD_AXIS_RIGHT_X] = 0.25f;
                data.deadzones[GLFW_GAMEPAD_AXIS_LEFT_Y] = 0.25f;
                data.deadzones[GLFW_GAMEPAD_AXIS_RIGHT_Y] = 0.25f;
                data.deadzones[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] = 0.10f;
                data.deadzones[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] = 0.10f;
            }
        }

        Gamepads::~Gamepads() {}

        void Gamepads::Update() 
        {
            __INTERNAL__CheckForConnectedGamepads();

            if(m_numConnectedGamepads > 0) 
            {
                __INTERNAL__UpdateButtons();
            }
        }

        BOOL Gamepads::HasChanged() 
        {
            BOOL result = m_hasChanged;
            m_hasChanged = false;
            return result;
        }

        void Gamepads::__INTERNAL__CheckForConnectedGamepads() 
        {
            for(ui8 i = 0; i < NUM_MAX_CONNECTED_GAMEPADS; ++i) 
            {
                GLFWCall(BOOL result = glfwJoystickPresent(i) == GLFW_TRUE);
                GamepadData& gamepad = m_data[i];

                if(gamepad.connected != result) 
                {
                    if(result) 
                    {
                        GLFWCall(STRING name = glfwGetJoystickName(i));
                        if(name.find("Playstation") != name.npos || name.find("playstation") != name.npos) 
                        {
                            gamepad.type = GamepadType::PLAYSTATION;
                        } 
                        else 
                        {
                            gamepad.type = GamepadType::XBOX;
                        }

                        DEBUG_LOG("Gamepad connected! UID: ", std::to_string(i), " NAME: ", name, " TYPE: ", gamepad.type == GamepadType::XBOX ? "Xbox" : "Playstation");
                        m_numConnectedGamepads++;
                    } 
                    else 
                    {
                        DEBUG_LOG("Gamepad disconnected! UID", std::to_string(i));
                        m_numConnectedGamepads--;
                    }
                }

                gamepad.connected = result;
            }
        }

        void Gamepads::__INTERNAL__UpdateButtons() 
        {
            for(ui8 i = 0; i < NUM_MAX_CONNECTED_GAMEPADS; ++i) 
            {
                GamepadData& data = m_data[i];
                if(data.connected) 
                {
                    __INTERNAL__UpdateGamepadData(data);
                }
            }
        }

        void Gamepads::__INTERNAL__UpdateGamepadData(GamepadData& data) 
        {
            GLFWgamepadstate state;
            GLFWCall(int result = glfwGetGamepadState(data.id, &state));
            if (result) 
            { 
                for(ui8 k = 0; k < NUM_GAMEPAD_BUTTONS; ++k) 
                {
                    BOOL result = state.buttons[k] == GLFW_PRESS ? true : false;

                    if(result != data.buttons[k].current) 
                    {
                        m_hasChanged = true;
                    }

                    data.buttons[k].last = data.buttons[k].current;   
                    data.buttons[k].current = result;
                }

                for(ui8 k = 0; k < NUM_GAMEPAD_AXIS_AMOUNT; ++k) 
                {
                    data.axes[k] = state.axes[k];
                }

                //zero values that is between a small number
                __INTERNAL__ApplyAxisDeadzone(data, GLFW_GAMEPAD_AXIS_LEFT_X);
                __INTERNAL__ApplyAxisDeadzone(data, GLFW_GAMEPAD_AXIS_LEFT_Y);
                __INTERNAL__ApplyAxisDeadzone(data, GLFW_GAMEPAD_AXIS_RIGHT_X);
                __INTERNAL__ApplyAxisDeadzone(data, GLFW_GAMEPAD_AXIS_RIGHT_Y);
                __INTERNAL__ApplyAxisDeadzone(data, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER);
                __INTERNAL__ApplyAxisDeadzone(data, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER);
            }
        }

        void Gamepads::__INTERNAL__ApplyAxisDeadzone(GamepadData& data, int axis) 
        {
            FLOAT deadzone = data.deadzones[axis];
            FLOAT& curAxis = data.axes[axis];
            FLOAT invert = 1;
            switch (axis)
            {
                case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER:
                case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER:
                {
                    FLOAT min = -1 + (2 * deadzone); //-0.6
                    FLOAT max = 1.0f;
                    if(curAxis < min) 
                    {
                        curAxis = 0.0f;
                    } 
                    else 
                    {
                        curAxis = (curAxis - min) / (max - min);
                        m_hasChanged = true;
                    }
                }   break;
                case GLFW_GAMEPAD_AXIS_LEFT_Y:
                    invert = data.invertYAxis[0] ? 1 : -1;
                    goto calc;
                case GLFW_GAMEPAD_AXIS_RIGHT_Y:
                    invert = data.invertYAxis[1] ? 1 : -1;
                    goto calc;
                default:
                {
                    calc:
                    FLOAT value = 1 * deadzone;
                    if(curAxis < value && curAxis > -value) 
                    {
                        curAxis = 0.0f;
                    } 
                    else 
                    {
                        if(curAxis > 0.0f) 
                        {
                            curAxis = (curAxis - value) / (1 - value);
                        }
                        else
                        {
                            curAxis = (curAxis + value) / (1 - value);
                        }
                        curAxis *= invert;
                        m_hasChanged = true;
                    }
                }   break;
            }
        }
    }   
}