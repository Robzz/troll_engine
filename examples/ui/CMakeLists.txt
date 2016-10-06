add_executable(ui main.cpp vs.glsl fs.glsl)
target_link_libraries(ui TrollEngine ${TrollEngine_LIBRARIES})
set_property(TARGET ui PROPERTY CXX_STANDARD 14)
