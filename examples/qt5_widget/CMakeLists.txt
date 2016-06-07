cmake_minimum_required(VERSION 2.9)

include_directories(${TrollEngine_INCLUDE_DIRECTORIES})

qt5_wrap_ui(UI_HEADERS mainwindow.ui)
add_executable(qt5_widget main.cpp mainwindow.cpp mainwindow.h mainwindow.ui glwidget.h
                          glwidget.cpp vs.glsl fs.glsl)
target_link_libraries(qt5_widget TrollEngine ${TrollEngine_LIBRARIES})
set_property(TARGET qt5_widget PROPERTY CXX_STANDARD 14)
