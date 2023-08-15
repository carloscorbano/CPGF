#pragma once

#include <stdio.h>

namespace CPGFramework 
{
    namespace Console 
    {
        namespace Color 
        {
            static const char* RESET = "\033[0m";
            static const char* BLACK = "\033[0;30m";
            static const char* RED = "\033[0;31m";
            static const char* GREEN = "\033[0;32m";
            static const char* YELLOW = "\033[0;33m";
            static const char* BLUE = "\033[0;34m";
            static const char* WHITE = "\033[0;37m";
        }

        inline static void tint_black()   { printf(Color::BLACK);     }
        inline static void tint_red()     { printf(Color::RED);       }
        inline static void tint_green()   { printf(Color::GREEN);     }
        inline static void tint_yellow()  { printf(Color::YELLOW);    }
        inline static void tint_blue()    { printf(Color::BLUE);      }
        inline static void tint_white()   { printf(Color::WHITE);     }
        inline static void reset_color()  { printf(Color::RESET);     }
        
    } // namespace Console
} // namespace CeosFramework