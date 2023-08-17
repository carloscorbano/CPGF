#pragma once

#include "input_common.hpp"

namespace CPGFramework
{
    class Engine;
    namespace Input
    {
        struct KeyboardMouseData 
        {
            InputData keyboardKeys[NUM_KEYBOARD_KEYS];
            InputData mouseButtons[NUM_MOUSE_BUTTONS];
            /// @brief 0 - LAST, 1 - CURRENT
            VEC2 cursorPos[2];
            /// @brief 0 - LAST, 1 - CURRENT, 2 - NEXT (because of thread sync)
            VEC2 scrollVal[3]; 
        };

        class KeyboardMouse 
        {
            KeyboardMouse();
        public:
            friend class InputClass;
            ~KeyboardMouse();

            /// @brief Setup the keyboard and input 
            /// @param window The window to be bounded.
            void Setup(Engine& engine);
            /// @brief Updates the keyboard mouse state.
            void Update();
            /// @brief Poll the changed state and retrieve it.
            /// @return If the state has changed.
            BOOL HasChanged();
        private:
            /// @brief Callback to handle scroll data changes (glfw don't have another way to retrieve it).
            static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        private:
            /// @brief Update keyboard
            void __INTERNAL__UpdateKeys();
            /// @brief Update Mouse
            void __INTERNAL__UpdateMouse();
        private:
            KeyboardMouseData m_data;
            BOOL m_hasChanged;
            GLFWwindow* m_window;
        };
    }   
}