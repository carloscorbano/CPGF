#pragma once

#include "input_defs.hpp"

namespace CPGFramework
{
     namespace Input 
    {
        class InputClass;
        
        struct InputData 
        {
            BOOL last, current;
            InputData() : last(false), current(false) {}

            /// @brief Check if this input was pressed.
            /// @return The state.
            inline BOOL IsPressed()     const { return !last && current;    }
            /// @brief Check if this input is beeing holded.
            /// @return The state.
            inline BOOL IsHolded()      const { return last && current;     }
            /// @brief Check if this input was released.
            /// @return The state.
            inline BOOL IsReleased()    const { return last && !current;    }
        };
    }
}