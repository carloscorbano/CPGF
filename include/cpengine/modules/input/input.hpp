#pragma once

#include "../iengine_module.hpp"
#include "keyboard_mouse.hpp"
#include "gamepads.hpp"

namespace CPGFramework
{
    class Engine;
    namespace Input
    {
        class InputClass : public IEngineModule
        {
        public:
            friend class CPGFramework::Engine;
            InputClass(Engine* engine);
            ~InputClass();

            /// @brief Returns if the key was pressed.
			/// @param key The key to be checked
			/// @return True if pressed, false if it was not pressed.
			const BOOL IsKeyPressed(const Key& key) const;
			/// @brief Returns if the key is beeing holded.
			/// @param key The key to be checked
			/// @return True if is holded, false if it is not beeing holded.
			const BOOL IsKeyHolded(const Key& key) const;
			/// @brief Returns if the key was released.
			/// @param key The key to be checked
			/// @return True if released, false if it was not released.
			const BOOL IsKeyReleased(const Key& key) const;

			/// @brief Returns if the button was pressed.
			/// @param button The button to be checked
			/// @return True if pressed, false if it was not pressed.
			const BOOL IsMouseButtonPressed(const MouseButton& button) const;
			/// @brief Returns if the button is beeing holded.
			/// @param button The button to be checked
			/// @return True if is holded, false if it is not beeing holded.
			const BOOL IsMouseButtonHolded(const MouseButton& button) const;
			/// @brief Returns if the button was released.
			/// @param button The button to be checked
			/// @return True if released, false if it was not released.
			const BOOL IsMouseButtonReleased(const MouseButton& button) const;

			/// @brief The mouse position in the screen.
			/// @return Returns the mouse position in the screen.
			const VEC2 GetMousePosition() const;
			/// @brief The mouse position delta, how much was moved from the last position.
			/// @return The mouse position delta.
			const VEC2 GetMousePositionDelta();
			/// @brief The mouse scroll value.
			/// @return The mouse scroll value.
			const VEC2 GetMouseScroll() const;

			/// @brief Check if the gamepad of the given UID is connected.
			/// @param UID The gamepad UID, must be below the constant 'NUM_MAX_CONNECTED_GAMEPADS'
			/// @return If the gamepad is connected.
			const BOOL IsGamepadConnected(const ui8& UID) const;
			/// @brief Check the gamepad button state.
			/// @param UID The gamepad UID, must be below the constant 'NUM_MAX_CONNECTED_GAMEPADS'
			/// @param button The button to be checked.
			/// @return If is pressed.
			const BOOL IsGamepadButtonPressed(const ui8& UID, const GamepadButton& button) const;
			/// @brief Check the gamepad button state.
			/// @param UID The gamepad UID, must be below the constant 'NUM_MAX_CONNECTED_GAMEPADS'
			/// @param button The button to be checked.
			/// @return If is holded.
			const BOOL IsGamepadButtonHolded(const ui8& UID, const GamepadButton& button) const;
			/// @brief Check the gamepad button state.
			/// @param UID The gamepad UID, must be below the constant 'NUM_MAX_CONNECTED_GAMEPADS'
			/// @param button The button to be checked.
			/// @return If is released.
			const BOOL IsGamepadButtonReleased(const ui8& UID, const GamepadButton& button) const;
			
			/// @brief Retrieves the gamepad axis value.
			/// @param UID The gamepad UID, must be below the constant 'NUM_MAX_CONNECTED_GAMEPADS'
			/// @param axis The axis to be retrieved.
			/// @return The axis value
			const FLOAT GetGamepadAxis(const ui8& UID, const GamepadAxes& axis) const;
			/// @brief Retrieve the gamepad type.
			/// @param UID The gamepad UID, must be below the constant 'NUM_MAX_CONNECTED_GAMEPADS'
			/// @return The gamepad type.
			const GamepadType GetGamepadType(const ui8& UID) const;

            KeyboardMouse& GetKeyboarMouseObj();
            Gamepads& GetGamepadsObj();
        private:
            void Initialize() override;
			/// @brief Updates the input states.
			void Update() override;
        private:
            KeyboardMouse m_keyboardMouse;
			Gamepads m_gamepads;
			InputType m_inputType;
        };
    }   
}