cmake_minimum_required(VERSION 3.3)

include(ExternalProject)

# Use GLFW CMake build system
add_subdirectory(./lib/glfw-3.1.2)

ExternalProject_Add(soil_build
                    PREFIX lib/soil
                    SOURCE_DIR lib/soil
                    CONFIGURE_COMMAND \;
                    BINARY_DIR lib/soil/projects/makefile
                    BUILD_COMMAND make
                    INSTALL_COMMAND \;)
add_library(soil STATIC IMPORTED)
set_property(TARGET soil PROPERTY IMPORTED_LOCATION lib/soil/lib/libSOIL.a)

#add_definitions(-DGL_TRACE)
add_definitions(-DDEBUG)
add_definitions(-DGLM_SWIZZLE)

# Define project
set(SOURCES
    src/gl_core_3_3.cpp
    src/main.cpp
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
)

add_executable(Projet ${SOURCES})
add_dependencies(soil soil_build)
#add_dependencies(Projet soil glfw)
set_target_properties(Projet PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
# Enable C++11
set_property(TARGET Projet PROPERTY CXX_STANDARD 11)
target_compile_options(Projet PUBLIC -Wall -Wextra -Weffc++)

include_directories(include lib/glfw-3.1.2/include lib/glm-0.9.7.1 lib/bitmap)
target_link_libraries(Projet glfw ${GLFW_LIBRARIES})

# Archive target
set(ARCHIVE_FILES
    include
    src
    assets
    lib
    shaders
    CMakeLists.txt
    rapport.tex
    rapport.pdf
    )
add_custom_command(OUTPUT chavignat_lmg.tar.gz
                   COMMAND tar -acf chavignat_lmg.tar.gz ${ARCHIVE_FILES})
add_custom_target(archive DEPENDS chavignat_lmg.tar.gz)
