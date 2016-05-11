cmake_minimum_required(VERSION 2.8)
project(TROLL CXX)
include(ExternalProject)

ExternalProject_Add(
    glLoadGen
    PREFIX glLoadGen
    HG_REPOSITORY https://bitbucket.org/alfonse/glloadgen
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
find_program(LUA_EXECUTABLE lua5.1)
if(${LUA_EXECUTABLE} STREQUAL LUA_EXECUTABLE-NOTFOUND)
    message(FATAL_ERROR "Lua 5.1 not found")
endif()

set(troll_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(troll_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Teach the build system how to generate the gl loaders
add_custom_target(generate_gl_loader
                  DEPENDS glLoadGen ${troll_src_dir}/gl_core_3_3.cpp ${troll_include_dir}/gl_core_3_3.h)
add_custom_command(OUTPUT ${troll_src_dir}/gl_core_3_3.cpp ${troll_include_dir}/gl_core_3_3.h
                   COMMAND ${LUA_EXECUTABLE} glLoadGen/src/glLoadGen/LoadGen.lua -style=noload_c -spec=gl -version=3.3 -profile=core core_3_3 -extfile=exts.txt
                   COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/gl_core_3_3.c ${troll_src_dir}/gl_core_3_3.cpp
                   COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/gl_core_3_3.h ${troll_include_dir}/gl_core_3_3.h)

# Remove the loader files on clean
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${troll_src_dir}/gl_core_3_3.cpp\
                                                                 ${troll_include_dir}/gl_core_3_3.h")

find_package(PkgConfig REQUIRED)
find_package(Boost 1.58 REQUIRED)
#find_package(BISON REQUIRED)
#find_package(FLEX REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_library(FREEIMAGE_LIBRARY freeimage)
find_library(ASSIMP_LIBRARY assimp)
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
#flex_target(obj_lexer ${troll_src_dir}/wavefront_obj.l ${troll_src_dir}/obj_lexer.cpp)
#bison_target(obj_parser ${troll_src_dir}/wavefront_obj.ypp ${troll_src_dir}/obj_parser.cpp
#                COMPILE_FLAGS "-b include"
#                DEFINES_FILE ${troll_include_dir}/obj_parser.h)
#add_flex_bison_dependency(obj_lexer obj_parser)

# Place generated header files in the appropriate dir and clean them correctly
#add_custom_target(move_bison_files
#                  DEPENDS ${troll_src_dir}/obj_parser.cpp ${troll_include_dir}/stack.hh)
#add_custom_command(OUTPUT ${troll_include_dir}/stack.hh
#                   COMMAND mv ${troll_src_dir}/stack.hh ${troll_include_dir})
#get_directory_property(TEMP DIRECTORY ${CMAKE_HOME_DIRECTORY} ADDITIONAL_MAKE_CLEAN_FILES)
#set_directory_properties(PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${TEMP}\
#                                                              ${troll_include_dir}/stack.hh\
#                                                              ${CMAKE_CURRENT_SOURCE_DIR}/glloadgen")

# Silence warnings in Flex and Bison files
#set_source_files_properties(${FLEX_obj_lexer_OUTPUTS} ${BISON_obj_parser_OUTPUT_SOURCE} PROPERTIES COMPILE_FLAGS "-w")

# Engine sources
set(TROLL_SOURCES
    ${troll_src_dir}/gl_core_3_3.cpp
    ${troll_src_dir}/troll_engine.cpp
    ${troll_src_dir}/window.cpp
    ${troll_src_dir}/shader.cpp
    ${troll_src_dir}/program.cpp
    ${troll_src_dir}/debug.cpp
    ${troll_src_dir}/vbo.cpp
    ${troll_src_dir}/vao.cpp
    ${troll_src_dir}/matrixstack.cpp
    ${troll_src_dir}/scenegraph.cpp
    ${troll_src_dir}/input.cpp
    ${troll_src_dir}/camera.cpp
    ${troll_src_dir}/texture.cpp
    ${troll_src_dir}/mesh.cpp
    #${troll_src_dir}/obj.cpp
    ${troll_src_dir}/attribute.cpp
    ${troll_src_dir}/fbo.cpp
    ${troll_src_dir}/image.cpp
    ${troll_src_dir}/transform.cpp
    ${troll_src_dir}/utility.cpp
    ${troll_src_dir}/sceneimporter.cpp
    #${FLEX_obj_lexer_INPUT}
    #${FLEX_obj_lexer_OUTPUTS}
    #${BISON_obj_parser_INPUT}
    #${BISON_obj_parser_OUTPUT_SOURCE}
)

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
    #${troll_include_dir}/obj.h
    #${troll_include_dir}/obj_lexer.h
    ${troll_include_dir}/attribute.h
    ${troll_include_dir}/fbo.h
    ${troll_include_dir}/image.h
    ${troll_include_dir}/transform.h
    ${troll_include_dir}/utility.h
    ${troll_include_dir}/sceneimporter.h
    #${BISON_obj_parser_OUTPUT_HEADER}
)

add_library(TrollEngine STATIC ${TROLL_SOURCES} ${TROLL_HEADERS})
include_directories("${troll_include_dir}"
                    "${GLFW_INCLUDE_DIRS}"
                    "${Boost_INCLUDE_DIRS}")

set_target_properties(TrollEngine PROPERTIES OUTPUT_NAME troll)

set(TROLL_LIBRARIES ${GLFW_STATIC_LIBRARIES} ${FREEIMAGE_LIBRARY} ${Boost_LIBRARIES} ${ASSIMP_LIBRARY})
target_link_libraries(TrollEngine ${TROLL_LIBRARIES})

add_dependencies(TrollEngine generate_gl_loader)

# Enable C++14 and a proper metric fuckton of extra warnings
set_property(TARGET TrollEngine PROPERTY CXX_STANDARD 14)
target_compile_options(TrollEngine PUBLIC -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wno-unused)
# Silence some expected warnings
set_source_files_properties(${troll_src_dir}/gl_core_3_3.cpp PROPERTIES COMPILE_FLAGS "-Wno-old-style-cast -Wno-sign-conversion -Wno-missing-declarations")
set_source_files_properties(${troll_src_dir}/debug.cpp PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter")

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
