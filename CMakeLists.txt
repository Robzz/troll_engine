cmake_minimum_required(VERSION 3.3)
include(ExternalProject)

# Build GLEW
ExternalProject_Add(glew_build
SOURCE_DIR lib/glew-1.13.0
CONFIGURE_COMMAND ""
BUILD_COMMAND make
INSTALL_COMMAND ""
BUILD_IN_SOURCE 1
)
# Define GLEW library
ExternalProject_Get_Property(glew_build source_dir)
add_library(glew STATIC IMPORTED)
set_property(TARGET glew PROPERTY IMPORTED_LOCATION ${source_dir}/lib/libGLEW.a)

# Use GLFW CMake build system
add_subdirectory(./lib/glfw-3.1.2)

# Define project
set(SOURCES
    src/main.cpp
    src/window.cpp
    src/shader.cpp
    src/debug.cpp
)

add_executable(Projet ${SOURCES})
add_dependencies(Projet glew_build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Enable C++11
set_property(TARGET Projet PROPERTY CXX_STANDARD 11)

include_directories(include lib/glew-1.13.0/include lib/glfw-3.1.2/include lib/glm-0.9.7.1)
target_link_libraries(Projet glew glfw ${GLFW_LIBRARIES})
