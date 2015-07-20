cmake_minimum_required(VERSION 2.6)
project(BDirScanner)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
enable_testing()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.55 REQUIRED
COMPONENTS
signals
system
filesystem
test_exec_monitor
unit_test_framework
)

include_directories("${Boost_INCLUDE_DIRS}")
include_directories("include")
include_directories("${CMAKE_BINARY_DIR}/externals/fnv")

add_subdirectory(externals)
add_subdirectory(dirit)
add_subdirectory(tests)


