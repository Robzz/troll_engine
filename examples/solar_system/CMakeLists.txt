cmake_minimum_required(VERSION 2.8)

include_directories(${TrollEngine_INCLUDE_DIRECTORIES})

add_executable(solar_system main.cpp)
target_link_libraries(solar_system TrollEngine ${TrollEngine_LIBRARIES})
set_property(TARGET solar_system PROPERTY CXX_STANDARD 14)
