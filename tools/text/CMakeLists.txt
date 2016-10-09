find_package(Freetype REQUIRED)

set(TOOL_TEXT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freetype.cpp)
set(TOOL_TEXT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/freetype.h)

add_executable(text ${TOOL_TEXT_SOURCES} ${TOOL_TEXT_HEADERS})
target_include_directories(text PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(text ${FREETYPE_LIBRARIES} ${YAML_CPP_LIBRARIES} image)

set_property(TARGET text PROPERTY CXX_STANDARD 14)
