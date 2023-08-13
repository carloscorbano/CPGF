cmake_minimum_required(VERSION 3.27.1)
project(CPGF VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_DEBUG_POSTFIX d)
option(INCLUDE_EXAMPLE "If the example project will be builded too" ON)

set(ROOT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_INSTALL_PREFIX install)

add_subdirectory(submodules)
add_subdirectory(include/cpengine)

if(INCLUDE_EXAMPLE)
    add_subdirectory(example)
endif()
