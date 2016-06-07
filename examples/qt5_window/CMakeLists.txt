cmake_minimum_required(VERSION 2.9)

include_directories(${TrollEngine_INCLUDE_DIRECTORIES})

add_executable(qt5_window main.cpp mainwindow.cpp mainwindow.h vs.glsl fs.glsl)
target_link_libraries(qt5_window TrollEngine ${TrollEngine_LIBRARIES})
set_property(TARGET qt5_window PROPERTY CXX_STANDARD 14)
