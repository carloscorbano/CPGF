target_sources(cp_engine
    PUBLIC
        iengine_module.hpp
        module_container.hpp
)

add_subdirectory(graphics)
add_subdirectory(threading)
add_subdirectory(time)
add_subdirectory(input)