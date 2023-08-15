target_sources(cp_engine
    PUBLIC
        dll.hpp
        typedefs.hpp
)

target_include_directories(cp_engine 
    PUBLIC 
        ${ROOT_DIR}/submodules/glm/glm
)

target_link_libraries(cp_engine
    PUBLIC
        glm
)