#pragma once

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#ifdef BUILD_DLL
#define CPENGINE_API DLL_EXPORT
#else
#define CPENGINE_API DLL_IMPORT
#endif