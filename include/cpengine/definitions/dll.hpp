#pragma once

#ifndef CPGF_API
    #ifdef BUILD_DLL
        #define CPGF_API __declspec(dllexport)
    #else
        #define CPGF_API __declspec(dllimport)
    #endif
#endif