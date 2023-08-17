#include "input.hpp"
#include "../../debug/debug.hpp"

namespace CPGFramework
{
    namespace Input
    {
        InputClass::InputClass(Engine* engine) 
            : IEngineModule(engine)
        {
            m_inputType = InputType::Keyboard_Mouse;
        }

        InputClass::~InputClass() {}

        void InputClass::Initialize()
        {
            m_keyboardMouse.Setup(GetEngineRef());
        }
        
        void InputClass::Update() 
        {
            m_keyboardMouse.Update();
            m_gamepads.Update();

            if(m_keyboardMouse.HasChanged() && m_inputType != InputType::Keyboard_Mouse) 
            {
                m_inputType = InputType::Keyboard_Mouse;
                DEBUG_WARNING("Input changed to keyboard and mouse.");
            } 
            
            if(m_gamepads.HasChanged() && m_inputType != InputType::Gamepad) 
            {
                m_inputType = InputType::Gamepad;
                DEBUG_WARNING("Input changed to gamepad.");
            }
        }

        const BOOL InputClass::IsKeyPressed(const Key& key) const 
        {
            return m_keyboardMouse.m_data.keyboardKeys[(int)key].IsPressed();
        }
        
        const BOOL InputClass::IsKeyHolded(const Key& key) const 
        {
            return m_keyboardMouse.m_data.keyboardKeys[(int)key].IsHolded();
        }
        
        const BOOL InputClass::IsKeyReleased(const Key& key) const 
        {
            return m_keyboardMouse.m_data.keyboardKeys[(int)key].IsReleased();
        }

        const BOOL InputClass::IsMouseButtonPressed(const MouseButton& button) const 
        {
            return m_keyboardMouse.m_data.mouseButtons[(int)button].IsPressed();
        }

        const BOOL InputClass::IsMouseButtonHolded(const MouseButton& button) const 
        {
            return m_keyboardMouse.m_data.mouseButtons[(int)button].IsHolded();
        }

        const BOOL InputClass::IsMouseButtonReleased(const MouseButton& button) const 
        {
            return m_keyboardMouse.m_data.mouseButtons[(int)button].IsReleased();
        }

        const VEC2 InputClass::GetMousePosition() const 
        {
            return m_keyboardMouse.m_data.cursorPos[VAL_CURRENT];
        }

        const VEC2 InputClass::GetMousePositionDelta() 
        {
            return m_keyboardMouse.m_data.cursorPos[VAL_CURRENT] - m_keyboardMouse.m_data.cursorPos[VAL_LAST];
        }

        const VEC2 InputClass::GetMouseScroll() const 
        {
            return m_keyboardMouse.m_data.scrollVal[VAL_CURRENT];
        }

        const BOOL InputClass::IsGamepadConnected(const ui8& UID) const 
        {
            if(UID >= NUM_MAX_CONNECTED_GAMEPADS) return false;
            return m_gamepads.m_data[UID].connected;
        }

        const BOOL InputClass::IsGamepadButtonPressed(const ui8& UID, const GamepadButton& button) const 
        {
            if(!IsGamepadConnected(UID)) return false;
            return m_gamepads.m_data[UID].buttons[(int)button].IsPressed();
        }

        const BOOL InputClass::IsGamepadButtonHolded(const ui8& UID, const GamepadButton& button) const 
        {
            if(!IsGamepadConnected(UID)) return false;
            return m_gamepads.m_data[UID].buttons[(int)button].IsHolded();
        }

        const BOOL InputClass::IsGamepadButtonReleased(const ui8& UID, const GamepadButton& button) const 
        {
            if(!IsGamepadConnected(UID)) return false;
            return m_gamepads.m_data[UID].buttons[(int)button].IsReleased();
        }

        const FLOAT InputClass::GetGamepadAxis(const ui8& UID, const GamepadAxes& axis) const 
        {
            if(!IsGamepadConnected(UID)) return 0.0f;
            return m_gamepads.m_data[UID].axes[(int)axis];
        }

        const GamepadType InputClass::GetGamepadType(const ui8& UID) const 
        {
            if(!IsGamepadConnected(UID)) return GamepadType::UNKOWN;
            return m_gamepads.m_data[UID].type;
        }

        KeyboardMouse& InputClass::GetKeyboarMouseObj()
        {
            return m_keyboardMouse;
        }
        
        Gamepads& InputClass::GetGamepadsObj()
        {
            return m_gamepads;
        }
    }   
}