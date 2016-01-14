cmake_minimum_required(VERSION 3.3)

# Use GLFW CMake build system
add_subdirectory(./lib/glfw-3.1.2)

#add_definitions(-DGL_TRACE)
add_definitions(-DDEBUG)

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
)

add_executable(Projet ${SOURCES})
set_target_properties(Projet PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
# Enable C++11
set_property(TARGET Projet PROPERTY CXX_STANDARD 11)

include_directories(include lib/glfw-3.1.2/include lib/glm-0.9.7.1)
target_link_libraries(Projet glfw ${GLFW_LIBRARIES})
