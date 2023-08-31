target_sources(cp_engine
    PUBLIC
        typedefs.hpp
        dll.hpp
        stringify.hpp
        math.hpp
        thread.hpp
        constants.hpp
        default.hpp
        space.hpp
        mat4.hpp
)

target_include_directories(cp_engine 
    PUBLIC 
        ${ROOT_DIR}/submodules/glm/glm
)

target_link_libraries(cp_engine
    PUBLIC
        glm
)