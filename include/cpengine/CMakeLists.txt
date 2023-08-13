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

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(cp_engine PRIVATE _NDEBUG)
else()
    target_compile_definitions(cp_engine PRIVATE _RELEASE)
endif()

add_subdirectory(definitions)

target_sources(cp_engine
    PRIVATE
        engine.cpp
    PUBLIC
        engine.hpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(cp_engine
    PUBLIC
        opengl32
        libglew_shared
        glfw
        glm
        cereal
        stb
        EnTT::EnTT
)

target_include_directories(cp_engine 
    PUBLIC 
        ../../submodules/glew/include
        ../../submodules/glfw/include
        ../../submodules/glm/glm
        ../../submodules/cereal/include
        ../../submodules/stb
        ../../submodules/entt/src
)

target_link_directories(cp_engine
    PUBLIC
        ${CMAKE_BINARY_DIR}/submodules/glew/lib/Debug       
        ${CMAKE_BINARY_DIR}/submodules/glfw/Debug
)