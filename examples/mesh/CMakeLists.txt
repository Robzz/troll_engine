cmake_minimum_required(VERSION 2.9)

include_directories(${TrollEngine_INCLUDE_DIRECTORIES})

add_executable(mesh main.cpp vs.glsl fs.glsl)
target_link_libraries(mesh TrollEngine ${TrollEngine_LIBRARIES})
set_property(TARGET mesh PROPERTY CXX_STANDARD 14)
