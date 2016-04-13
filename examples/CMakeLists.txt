cmake_minimum_required(VERSION 2.8)

option(BUILD_SOLAR_SYSTEM_EXAMPLE "Build solar system example" ON)

if(BUILD_SOLAR_SYSTEM_EXAMPLE)
    add_subdirectory(solar_system)
endif()
