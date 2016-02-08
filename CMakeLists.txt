cmake_minimum_required(VERSION 2.8)

project(MCGYVER CXX)

find_package(PkgConfig REQUIRED)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    #add_definitions(-DGL_TRACE)
    add_definitions(-DDEBUG)
endif()
add_definitions(-DGLM_SWIZZLE)

option(MCGYVER_BUILD_EXAMPLES "Build examples" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Generate parsers and lexers
flex_target(obj_lexer src/wavefront_obj.l src/obj_lexer.cpp)
bison_target(obj_parser src/wavefront_obj.ypp src/obj_parser.cpp
                COMPILE_FLAGS "-b include"
                DEFINES_FILE include/obj_parser.h)
add_flex_bison_dependency(obj_lexer obj_parser)

# Place generated header files in the appropriate dir and clean them correctly
add_custom_target(move_bison_files
                  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/obj_parser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/stack.hh)
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/stack.hh
                   COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/src/stack.hh ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_directory_properties(PROPERTY ADDITIONAL_MAKE_CLEAN_FILES include/stack.hh)

# Silence warnings in Flex and Bison files
set_source_files_properties(${FLEX_obj_lexer_OUTPUTS} ${BISON_obj_parser_OUTPUT_SOURCE} PROPERTIES COMPILE_FLAGS "-w")

# Engine sources
set(MCGYVER_SOURCES
    src/gl_core_3_3.cpp
    src/window.cpp
    src/shader.cpp
    src/program.cpp
    src/debug.cpp
    src/vbo.cpp
    src/vao.cpp
    src/matrixstack.cpp
    src/scenegraph.cpp
    src/input.cpp
    src/camera.cpp
    src/texture.cpp
    src/planet.cpp
    src/mesh.cpp
    src/obj.cpp
    ${FLEX_obj_lexer_INPUT}
    ${FLEX_obj_lexer_OUTPUTS}
    ${BISON_obj_parser_INPUT}
    ${BISON_obj_parser_OUTPUT_SOURCE}
)

set(MCGYVER_HEADERS
    include/gl_core_3_3.h
    include/window.h
    include/shader.h
    include/program.h
    include/debug.h
    include/vbo.h
    include/vao.h
    include/matrixstack.h
    include/scenegraph.h
    include/input.h
    include/camera.h
    include/texture.h
    include/planet.h
    include/mesh.h
    include/obj.h
    include/obj_lexer.h
    ${BISON_obj_parser_OUTPUT_HEADER}
)

add_library(McGyver STATIC ${MCGYVER_SOURCES} ${MCGYVER_HEADERS})
include_directories(McGyver "${MCGYVER_SOURCE_DIR}/include"
                            ${GLFW_INCLUDE_DIRS}
                            "${IMAGE_SOURCE_DIR}/include")

set_target_properties(McGyver PROPERTIES OUTPUT_NAME mcgyver)

target_link_libraries(McGyver glfw ${GLFW_STATIC_LIBRARIES} image)

add_subdirectory(image)
add_dependencies(McGyver image move_bison_files)

# Enable C++11 and extra warnings
set_property(TARGET McGyver PROPERTY CXX_STANDARD 11)
target_compile_options(McGyver PUBLIC -Wall -Wextra -Weffc++)

if(MCGYVER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Archive target
set(ARCHIVE_FILES
    include
    src
    examples
    doc
    lib
    shaders
    CMakeLists.txt
)

add_custom_command(OUTPUT mcgyver.tar.xz
    COMMAND tar -acf mcgyver.tar.xz ${ARCHIVE_FILES})
add_custom_target(archive DEPENDS mcgyver.tar.xz)
