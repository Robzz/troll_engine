find_package(Freetype)

set(TOOL_TEXT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/freetype.cpp)
set(TOOL_TEXT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/freetype.h)

add_executable(text ${TOOL_TEXT_SOURCES} ${TOOL_TEXT_HEADERS})
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(text ${FREETYPE_LIBRARIES} image)

set_property(TARGET text PROPERTY CXX_STANDARD 14)
