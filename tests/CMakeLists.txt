set(TESTSUITE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_utilities.cpp
)

add_executable(testsuite ${TESTSUITE_SOURCES})
target_link_libraries(testsuite TrollEngine ${TrollEngine_LIBRARIES})
set_property(TARGET testsuite PROPERTY CXX_STANDARD 14)
