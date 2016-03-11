cmake_minimum_required(VERSION 2.8) 

set(BUDDHA_HEADERS include/pov.h)
set(BUDDHA_SOURCES src/main.cpp src/pov.cpp)

include_directories(include ${Boost_INCLUDE_DIRS})

add_executable(buddha ${BUDDHA_SOURCES} ${BUDDHA_HEADERS})
target_link_libraries(buddha McGyver ${MCGYVER_LIBRARIES})
set_property(TARGET buddha PROPERTY CXX_STANDARD 11)
