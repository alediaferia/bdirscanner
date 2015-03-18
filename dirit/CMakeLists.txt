cmake_minimum_required(VERSION 2.6)
project(dinode)
set(dinode_sources
node.cpp
)


add_library(dinode STATIC ${dinode_sources})
target_link_libraries(dinode
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    fnv
)
add_dependencies(dinode fnv)
