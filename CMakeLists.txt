cmake_minimum_required(VERSION 2.8)
project(TROLL CXX)
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
                   COMMAND ${LUA_EXECUTABLE} glLoadGen/src/glloadgen/LoadGen.lua -style=noload_c -spec=gl -version=3.3 -profile=core core_3_3 -extfile=exts.txt
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
    add_definitions(-DDEBUG)
endif()
add_definitions(-DGLM_SWIZZLE)

option(TROLL_BUILD_EXAMPLES "Build examples" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Generate parsers and lexers
flex_target(obj_lexer ${CMAKE_CURRENT_SOURCE_DIR}/src/wavefront_obj.l ${CMAKE_CURRENT_SOURCE_DIR}/src/obj_lexer.cpp)
bison_target(obj_parser ${CMAKE_CURRENT_SOURCE_DIR}/src/wavefront_obj.ypp ${CMAKE_CURRENT_SOURCE_DIR}/src/obj_parser.cpp
                COMPILE_FLAGS "-b include"
                DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/obj_parser.h)
add_flex_bison_dependency(obj_lexer obj_parser)

# Place generated header files in the appropriate dir and clean them correctly
add_custom_target(move_bison_files
                  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/obj_parser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/stack.hh)
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/stack.hh
                   COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/src/stack.hh ${CMAKE_CURRENT_SOURCE_DIR}/include)
get_directory_property(TEMP DIRECTORY ${CMAKE_HOME_DIRECTORY} ADDITIONAL_MAKE_CLEAN_FILES)
set_directory_properties(PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${TEMP}\
                                                               include/stack.hh\
                                                               glloadgen")

# Silence warnings in Flex and Bison files
set_source_files_properties(${FLEX_obj_lexer_OUTPUTS} ${BISON_obj_parser_OUTPUT_SOURCE} PROPERTIES COMPILE_FLAGS "-w")

# Engine sources
set(TROLL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gl_core_3_3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/troll_engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/debug.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vbo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vao.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrixstack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scenegraph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/obj.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/attribute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fbo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utility.cpp
    ${FLEX_obj_lexer_INPUT}
    ${FLEX_obj_lexer_OUTPUTS}
    ${BISON_obj_parser_INPUT}
    ${BISON_obj_parser_OUTPUT_SOURCE}
)

set(troll_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TROLL_HEADERS
    ${troll_include_dir}/gl_core_3_3.h
    ${troll_include_dir}/troll_engine.h
    ${troll_include_dir}/window.h
    ${troll_include_dir}/shader.h
    ${troll_include_dir}/program.h
    ${troll_include_dir}/debug.h
    ${troll_include_dir}/vbo.h
    ${troll_include_dir}/vao.h
    ${troll_include_dir}/matrixstack.h
    ${troll_include_dir}/scenegraph.h
    ${troll_include_dir}/input.h
    ${troll_include_dir}/camera.h
    ${troll_include_dir}/texture.h
    ${troll_include_dir}/mesh.h
    ${troll_include_dir}/obj.h
    ${troll_include_dir}/obj_lexer.h
    ${troll_include_dir}/attribute.h
    ${troll_include_dir}/fbo.h
    ${troll_include_dir}/image.h
    ${troll_include_dir}/transform.h
    ${troll_include_dir}/utility.h
    ${BISON_obj_parser_OUTPUT_HEADER}
)

add_library(TrollEngine STATIC ${TROLL_SOURCES} ${TROLL_HEADERS})
include_directories("${troll_include_dir}"
                    "${GLFW_INCLUDE_DIRS}")

set_target_properties(TrollEngine PROPERTIES OUTPUT_NAME troll)

set(TROLL_LIBRARIES ${GLFW_STATIC_LIBRARIES} ${FREEIMAGE_LIBRARY} ${Boost_LIBRARIES})
target_link_libraries(TrollEngine ${TROLL_LIBRARIES})

add_dependencies(TrollEngine move_bison_files generate_gl_loader)

# Enable C++14 and a proper metric fuckton of extra warnings
set_property(TARGET TrollEngine PROPERTY CXX_STANDARD 14)
target_compile_options(TrollEngine PUBLIC -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wno-unused)
# Silence some expected warnings
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/gl_core_3_3.cpp PROPERTIES COMPILE_FLAGS "-Wno-old-style-cast -Wno-sign-conversion -Wno-missing-declarations")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/debug.cpp PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter")

if(TROLL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

export(TARGETS TrollEngine FILE troll-exports.cmake)

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

add_custom_command(OUTPUT troll_engine.tar.xz
    COMMAND tar -acf troll_engine.tar.xz ${ARCHIVE_FILES})
add_custom_target(archive DEPENDS troll_engine.tar.xz)
