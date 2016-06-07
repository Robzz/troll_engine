cmake_minimum_required(VERSION 2.9)

include_directories(${TrollEngine_INCLUDE_DIRECTORIES})

add_executable(glfw_window main.cpp vs.glsl fs.glsl)
target_link_libraries(glfw_window TrollEngine ${TrollEngine_LIBRARIES})
set_property(TARGET glfw_window PROPERTY CXX_STANDARD 14)
