target_sources(cp_engine
    PRIVATE
        window.cpp
    PUBLIC
        glfw.hpp
        window.hpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(cp_engine
    PUBLIC
        opengl32
        libglew_shared
        glfw
)

target_include_directories(cp_engine 
    PUBLIC
        "${ROOT_DIR}/submodules/glew/include"
        "${ROOT_DIR}/submodules/glfw/include/"
)