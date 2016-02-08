cmake_minimum_required(VERSION 2.8)

set(BUDDHA_SOURCES src/main.cpp)

add_executable(buddha ${BUDDHA_SOURCES})
target_link_libraries(buddha McGyver ${MCGYVER_LIBRARIES})
set_property(TARGET buddha PROPERTY CXX_STANDARD 11)
