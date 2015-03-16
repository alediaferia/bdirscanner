add_custom_target(
  setup_fnv
  COMMAND mkdir -p ${CMAKE_BINARY_DIR}/externals/fnv
)

add_custom_target(
   build_fnv
   COMMAND cp ${CMAKE_SOURCE_DIR}/externals/fnv/* . && make
   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/externals/fnv
   COMMENT "Buiding FNV hashing algorithm lib..."
)

# now create an imported static target visible to everybody
add_library(fnv STATIC IMPORTED GLOBAL)
# Import target fnv for configuration ""
set_property(TARGET fnv APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(fnv PROPERTIES IMPORTED_LOCATION_NOCONFIG "${CMAKE_BINARY_DIR}/externals/fnv/libfnv.a")

# now you can use fnv as if it were a regular cmake built target in your project
add_dependencies(build_fnv setup_fnv)
add_dependencies(fnv build_fnv)
