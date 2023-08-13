cmake_minimum_required(VERSION 3.27.1)
project(CPGF VERSION 0.0.1 LANGUAGES CXX)

add_executable(example main.cpp)
install(TARGETS example
        # for executables and dll on Win
        RUNTIME DESTINATION bin
        # shared libraries
        LIBRARY DESTINATION lib
        # for static libraries
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include)

target_include_directories(example PUBLIC ../include/)
target_link_libraries(example cp_engine)

include(../cmake/copy_dlls.cmake)

copy_runtime_dlls(example)