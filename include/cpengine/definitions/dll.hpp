#pragma once

#ifndef DLL_EXPORT
    #define DLL_EXPORT __declspec(dllexport)
#endif
#ifndef DLL_IMPORT
    #define DLL_IMPORT __declspec(dllimport)
#endif

#ifdef BUILD_DLL
    #ifndef CPGF_API
        #define CPGF_API DLL_EXPORT
    #endif
#else
    #ifndef CPGF_API
        #define CPGF_API DLL_IMPORT
    #endif
#endif