cmake_minimum_required(VERSION 2.8)
project(TROLL CXX)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    option(GENERATE_DOCS "Generate Doxygen documentation" OFF)
else()
    set(GENERATE_DOCS OFF)
endif()

if(GENERATE_DOCS)
    add_custom_target(doxygen_docs
                      COMMAND doxygen Doxyfile)
else()
    add_custom_target(doxygen_docs)
endif()

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build the test suite" OFF)
option(BUILD_TOOLS "Build the tools" ON)
option(WINDOW_GLFW "Use the GLFW library" ON)
option(WINDOW_QT5  "Use the Qt5 Library" OFF)

set(troll_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(troll_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Windowing system setup
if(WINDOW_GLFW AND WINDOW_QT5)
    message(FATAL_ERROR "You must choose only one window library (GLFW/Qt5).")
elseif(NOT WINDOW_GLFW AND NOT WINDOW_QT5)
    message(FATAL_ERROR "You must choose a window library (GLFW/Qt5).")
endif()

if(WINDOW_GLFW)
    find_package(glfw3 3.2 REQUIRED)
    set(TROLL_LIBRARIES glfw)
    add_definitions(-DTROLL_USE_GLFW)
elseif(WINDOW_QT5)
    find_package(Qt5Gui)
    find_package(Qt5Widgets)
    set(CMAKE_AUTOMOC ON)
    set(TROLL_LIBRARIES Qt5::Gui Qt5::Widgets)
    add_definitions(-DTROLL_USE_QT5)
endif()

add_subdirectory("image")

find_package(Boost 1.58 REQUIRED)
find_package(OpenGL)
find_package(glbinding REQUIRED)
find_library(ASSIMP_LIBRARY assimp)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    #add_definitions(-DGL_TRACE)
    add_definitions(-DDEBUG)
else()
    # Silence some expected warnings
    set_source_files_properties(src/debug.cpp PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter")
endif()
add_definitions(-DGLM_FORCE_SWIZZLE)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Engine sources
set(TROLL_SOURCES
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
    ${troll_src_dir}/attribute.cpp
    ${troll_src_dir}/fbo.cpp
    ${troll_src_dir}/transform.cpp
    ${troll_src_dir}/utility.cpp
    ${troll_src_dir}/sceneimporter.cpp
)

set(TROLL_HEADERS
    ${troll_include_dir}/gl_traits.h
    ${troll_include_dir}/troll_engine.h
    ${troll_include_dir}/window.h
    ${troll_include_dir}/shader.h
    ${troll_include_dir}/program.h
    ${troll_include_dir}/program.inl
    ${troll_include_dir}/debug.h
    ${troll_include_dir}/vbo.h
    ${troll_include_dir}/vbo.inl
    ${troll_include_dir}/vao.h
    ${troll_include_dir}/ubo.h
    ${troll_include_dir}/ubo.inl
    ${troll_include_dir}/matrixstack.h
    ${troll_include_dir}/scenegraph.h
    ${troll_include_dir}/input.h
    ${troll_include_dir}/camera.h
    ${troll_include_dir}/camera.inl
    ${troll_include_dir}/texture.h
    ${troll_include_dir}/texture.inl
    ${troll_include_dir}/mesh.h
    ${troll_include_dir}/attribute.h
    ${troll_include_dir}/fbo.h
    ${troll_include_dir}/fbo.inl
    ${troll_include_dir}/transform.h
    ${troll_include_dir}/transform.inl
    ${troll_include_dir}/utility.h
    ${troll_include_dir}/utility.inl
    ${troll_include_dir}/sceneimporter.h
)

set(TROLL_INCLUDE_DIRS ${TROLL_INCLUDE_DIRS} ${troll_include_dir} ${Boost_INCLUDE_DIRS})

add_library(TrollEngine STATIC ${TROLL_SOURCES} ${TROLL_HEADERS})
include_directories(${TROLL_INCLUDE_DIRS} "image")

set_target_properties(TrollEngine PROPERTIES OUTPUT_NAME troll)

set(TROLL_LIBRARIES ${TROLL_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${Boost_LIBRARIES} ${ASSIMP_LIBRARY} ${OPENGL_gl_LIBRARY} glbinding::glbinding image)
target_link_libraries(TrollEngine ${TROLL_LIBRARIES})

# Enable C++14 and some extra warnings
if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(cxx_extra_flags -Wdocumentation)
endif()
set_property(TARGET TrollEngine PROPERTY CXX_STANDARD 14)
target_compile_options(TrollEngine PUBLIC ${cxx_extra_flags} -Wall -Wextra)
# Silence some expected warnings
set_source_files_properties(${troll_src_dir}/debug.cpp PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter")

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    add_subdirectory("tests")
endif()

if(BUILD_TOOLS)
    add_subdirectory("tools")
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

add_custom_command(OUTPUT troll_engine.tar.xz
    COMMAND tar -acf troll_engine.tar.xz ${ARCHIVE_FILES})
add_custom_target(archive DEPENDS troll_engine.tar.xz)
