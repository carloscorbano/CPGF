#pragma once

#include "input_common.hpp"

namespace CPGFramework
{
    namespace Input
    {
        class InputClass;

        enum class GamepadType { UNKOWN, XBOX, PLAYSTATION };

        struct GamepadData 
        {
            ui8 id;
            BOOL connected;
            InputData buttons[NUM_GAMEPAD_BUTTONS];
            FLOAT axes[NUM_GAMEPAD_AXIS_AMOUNT];
            GamepadType type;
            FLOAT deadzones[NUM_GAMEPAD_AXIS_AMOUNT]; //percentage
            //0 - LEFT, 1 - RIGHT
            BOOL invertYAxis[2];
        };

        class Gamepads 
        {
            Gamepads();
        public:
            friend class InputClass;
            ~Gamepads();

            /// @brief Update the gamepads state.
            void Update();
            /// @brief Poll the changed state and retrieve it.
            /// @return If the state has changed.
            BOOL HasChanged();

        private:
            /// @brief Check for any changes in the gamepads connection state.
            void __INTERNAL__CheckForConnectedGamepads();
            /// @brief Update the gamepad buttons state.
            void __INTERNAL__UpdateButtons();
            /// @brief Helper method to update each gamepad state.
            /// @param data The current gamepad to be updated.
            void __INTERNAL__UpdateGamepadData(GamepadData& data);
            /// @brief Helper method to apply axis deadzone.
            /// @param data The current gamepad.
            /// @param axis The current axis.
            void __INTERNAL__ApplyAxisDeadzone(GamepadData& data, int axis);
        private:
            GamepadData m_data[NUM_MAX_CONNECTED_GAMEPADS];
            ui8 m_numConnectedGamepads;
            BOOL m_hasChanged;
        };
    }   
}