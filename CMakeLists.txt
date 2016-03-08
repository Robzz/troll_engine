cmake_minimum_required(VERSION 2.8)
project(MCGYVER CXX)
include(ExternalProject)

ExternalProject_Add(
    glLoadGen
    PREFIX glLoadGen
    DOWNLOAD_COMMAND hg clone https://bitbucket.org/alfonse/glloadgen
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
find_program(LUA_EXECUTABLE lua5.1)
if(${LUA_EXECUTABLE} STREQUAL LUA_EXECUTABLE-NOTFOUND)
    message(FATAL_ERROR "Lua 5.1 not found")
endif()

# Teach the build system how to generate the gl loaders
add_custom_target(generate_gl_loader
                  DEPENDS glLoadGen ${CMAKE_CURRENT_SOURCE_DIR}/src/gl_core_3_3.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/gl_core_3_3.h)
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/gl_core_3_3.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/gl_core_3_3.h
                   COMMAND ${LUA_EXECUTABLE} glLoadGen/src/glLoadGen/LoadGen.lua -style=noload_c -spec=gl -version=3.3 -profile=core core_3_3
                   COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/gl_core_3_3.c ${CMAKE_CURRENT_SOURCE_DIR}/src/gl_core_3_3.cpp
                   COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/gl_core_3_3.h ${CMAKE_CURRENT_SOURCE_DIR}/include/gl_core_3_3.h)

# Remove the loader files on clean
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/gl_core_3_3.cpp\
                                                                 ${CMAKE_CURRENT_SOURCE_DIR}/include/gl_core_3_3.h")

find_package(PkgConfig REQUIRED)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_library(FREEIMAGE_LIBRARY freeimage)
if(${FREEIMAGE_LIBRARY} STREQUAL FREEIMAGE_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "libfreeimage not found.")
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    #add_definitions(-DGL_TRACE)
    add_definitions(-DDEBUG)
else()
    # Silence some expected warnings
    set_source_files_properties(src/gl_core_3_3.cpp PROPERTIES COMPILE_FLAGS "-w -Wno-old-style-cast")
    set_source_files_properties(src/debug.cpp PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter")
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
get_directory_property(TEMP DIRECTORY ${CMAKE_HOME_DIRECTORY} ADDITIONAL_MAKE_CLEAN_FILES)
set_directory_properties(PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${TEMP} include/stack.hh")

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
    src/attribute.cpp
    src/fbo.cpp
    src/image.cpp
    src/transform.cpp
    src/utility.cpp
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
    include/attribute.h
    include/fbo.h
    include/image.h
    include/transform.h
    include/utility.h
    ${BISON_obj_parser_OUTPUT_HEADER}
)

add_library(McGyver STATIC ${MCGYVER_SOURCES} ${MCGYVER_HEADERS})
include_directories("${MCGYVER_SOURCE_DIR}/include"
                    ${GLFW_INCLUDE_DIRS})

set_target_properties(McGyver PROPERTIES OUTPUT_NAME mcgyver)

target_link_libraries(McGyver ${GLFW_STATIC_LIBRARIES} ${FREEIMAGE_LIBRARY})

add_dependencies(McGyver move_bison_files generate_gl_loader)

# Enable C++11 and a proper metric fuckton of extra warnings
set_property(TARGET McGyver PROPERTY CXX_STANDARD 11)
target_compile_options(McGyver PUBLIC -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wno-unused)

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
