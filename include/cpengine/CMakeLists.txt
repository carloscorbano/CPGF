cmake_minimum_required(VERSION 3.27.1)
project(CPGF VERSION 0.0.1 LANGUAGES CXX)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_library(cp_engine SHARED dllmain.cpp)
install(TARGETS cp_engine
        # for executables and dll on Win
        RUNTIME DESTINATION bin
        # shared libraries
        LIBRARY DESTINATION lib
        # for static libraries
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include)

target_compile_definitions(cp_engine PRIVATE BUILD_DLL)

add_subdirectory(definitions)

target_sources(cp_engine
    PRIVATE
        engine.cpp
    PUBLIC
        engine.hpp
)
