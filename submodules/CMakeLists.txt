cmake_minimum_required(VERSION 3.27.1)
project(CPGF VERSION 0.0.1 LANGUAGES CXX)

include(../cmake/submodules_updater.cmake)

set(ONLY_LIBS ON)

add_subdirectory(entt)

set(BUILD_DOC OFF)
set(BUILD_SANDBOX OFF)
set(SKIP_PERFORMANCE_COMPARISON ON)

add_subdirectory(cereal)

set(glew-cmake_BUILD_SHARED ON)
set(glew-cmake_BUILD_STATIC OFF)
set(ONLY_LIBS ON)

add_subdirectory(glew)

install(TARGETS libglew_shared
        # for executables and dll on Win
        RUNTIME DESTINATION bin
        # shared libraries
        LIBRARY DESTINATION lib
        # for static libraries
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

set(STB_TARGET_NAME stb)
set(STB_SOURCE_DIR ${SUBMODULES_DIR}stb)

add_library(${STB_TARGET_NAME} STATIC
    ${STB_SOURCE_DIR}/stb_c_lexer.h
    ${STB_SOURCE_DIR}/stb_connected_components.h
    ${STB_SOURCE_DIR}/stb_divide.h
    ${STB_SOURCE_DIR}/stb_ds.h
    ${STB_SOURCE_DIR}/stb_dxt.h
    ${STB_SOURCE_DIR}/stb_easy_font.h
    ${STB_SOURCE_DIR}/stb_herringbone_wang_tile.h
    ${STB_SOURCE_DIR}/stb_hexwave.h
    ${STB_SOURCE_DIR}/stb_image_resize.h
    ${STB_SOURCE_DIR}/stb_image_write.h
    ${STB_SOURCE_DIR}/stb_image.h
    ${STB_SOURCE_DIR}/stb_include.h
    ${STB_SOURCE_DIR}/stb_leakcheck.h
    ${STB_SOURCE_DIR}/stb_perlin.h
    ${STB_SOURCE_DIR}/stb_rect_pack.h
    ${STB_SOURCE_DIR}/stb_sprintf.h
    ${STB_SOURCE_DIR}/stb_textedit.h
    ${STB_SOURCE_DIR}/stb_tilemap_editor.h
    ${STB_SOURCE_DIR}/stb_truetype.h
    ${STB_SOURCE_DIR}/stb_vorbis.c
    ${STB_SOURCE_DIR}/stb_voxel_render.h
)

add_subdirectory(glm)