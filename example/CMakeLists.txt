cmake_minimum_required(VERSION 3.27.1)
project(CPGF VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable(example main.cpp)
install(TARGETS example
        # for executables and dll on Win
        RUNTIME DESTINATION bin
        # shared libraries
        LIBRARY DESTINATION lib
        # for static libraries
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include)

# COMPILE DEFINITIONS
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_definitions(_NDEBUG)
else()
    add_compile_definitions(_NRELEASE)
endif()

if (WIN32)
    if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set(SUBSYSTEM_LINKER_OPTIONS /SUBSYSTEM:CONSOLE)
    else()
        set(SUBSYSTEM_LINKER_OPTIONS /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
    endif()
else()
    if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set(SUBSYSTEM_LINKER_OPTIONS -mwindows)
    endif()
endif()

target_link_options(example PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})
# END COMPILE DEFINITIONS
target_compile_features(example PRIVATE cxx_std_20)
target_include_directories(example PUBLIC ../include/)
target_link_libraries(example cp_engine)

include(../cmake/copy_dlls.cmake)

copy_runtime_dlls(example)

target_sources(example
    PRIVATE
        game.cpp
    PUBLIC
        game.hpp
)