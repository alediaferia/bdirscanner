project(dirit_test)
include_directories(${CMAKE_SOURCE_DIR}/dirit)
set(dirit_test_srcs
    dirit_test.cpp
)
add_definitions(-D'TEST_DIR_SOURCE_PATH=${CMAKE_SOURCE_DIR}/tests/data/dirit_test')

add_executable(dirit_test ${dirit_test_srcs})
target_link_libraries(dirit_test
    dinode
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_test(NAME dirit_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests/dirit_test
    COMMAND dirit_test
)
add_dependencies(dirit_test dinode)
