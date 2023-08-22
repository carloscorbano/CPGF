target_sources(cp_engine
    PRIVATE
        resources.cpp
        filesystem.cpp
    PUBLIC
        resources.hpp
        filesystem.hpp
        resource_base.hpp
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(cp_engine
        PRIVATE
            NDEBUG_RESOURCES_PATH="${NDEBUG_RESOURCES_PATH}"
    )
endif()