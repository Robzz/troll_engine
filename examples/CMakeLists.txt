cmake_minimum_required(VERSION 2.8)

option(BUILD_SOLAR_SYSTEM_EXAMPLE "Build solar system example" ON)
option(BUILD_BUDDHA_EXAMPLE "Build buddha example" ON)

if(BUILD_SOLAR_SYSTEM_EXAMPLE)
    add_subdirectory(solar_system)
endif()

if(BUILD_BUDDHA_EXAMPLE)
    add_subdirectory(buddha)
endif()
