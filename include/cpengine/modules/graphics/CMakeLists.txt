target_sources(cp_engine
    PRIVATE
        window.cpp
        glBuffer.cpp
        material.cpp
        shader.cpp
        texture2D.cpp
        vertex_array.cpp
    PUBLIC
        glfw.hpp
        window.hpp
        glBuffer.hpp
        glTypes.hpp
        index_buffer.hpp
        material.hpp
        shader.hpp
        texture2D.hpp
        vertex_array.hpp
        vertex_buffer_layout.hpp
        vertex_buffer.hpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(cp_engine
    PUBLIC
        opengl32
        libglew_shared
        glfw
        stb
)

target_include_directories(cp_engine 
    PUBLIC
        "${ROOT_DIR}/submodules/glew/include"
        "${ROOT_DIR}/submodules/glfw/include"
        "${ROOT_DIR}/submodules/stb"
)