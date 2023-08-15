cmake_minimum_required(VERSION 3.27.1)
project(CPGF VERSION 0.0.1 LANGUAGES CXX)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_library(cp_engine SHARED dllmain.cpp)
install(TARGETS cp_engine
        # for executables and dll on Win
        RUNTIME DESTINATION bin
        # shared libraries
        LIBRARY DESTINATION lib
        # for static libraries
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include)

# target_include_directories(cp_engine 
#     PUBLIC 
#         "${ROOT_DIR}/submodules/glew/include"
#         "${ROOT_DIR}/submodules/glfw/include/"
#         # ${ROOT_DIR}/submodules/glm/glm
#         # ${ROOT_DIR}/submodules/cereal/include
#         # ${ROOT_DIR}/submodules/stb
#         # ${ROOT_DIR}/submodules/entt/src
# )

target_compile_definitions(cp_engine PRIVATE BUILD_DLL)
target_compile_features(cp_engine PRIVATE cxx_std_20)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(cp_engine PRIVATE _NDEBUG)
else()
    target_compile_definitions(cp_engine PRIVATE _RELEASE)
endif()

target_sources(cp_engine
    PRIVATE
        engine.cpp
    PUBLIC
        engine.hpp
)

# target_link_libraries(cp_engine
#     PUBLIC
#         opengl32
#         libglew_shared
#         glfw
#         glm
#         cereal
#         stb
#         EnTT::EnTT
# )

# target_link_directories(cp_engine
#     PUBLIC
#         ${CMAKE_BINARY_DIR}/submodules/glew/lib/Debug       
#         ${CMAKE_BINARY_DIR}/submodules/glfw/Debug
# )

add_subdirectory(definitions)
add_subdirectory(containers)
add_subdirectory(debug)
add_subdirectory(events)
add_subdirectory(modules)