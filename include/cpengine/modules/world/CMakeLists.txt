target_sources(cp_engine
    PRIVATE
        world.cpp
        entity.cpp
    PUBLIC
        world.hpp
        entity.hpp
        entt.hpp
)

target_link_libraries(cp_engine
    PUBLIC
        EnTT::EnTT
)

target_include_directories(cp_engine 
    PUBLIC 
        "${ROOT_DIR}/submodules/entt/src"
)