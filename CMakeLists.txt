cmake_minimum_required( VERSION 2.6 )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules )

enable_testing()

set( MEMORYCHECK_COMMAND /usr/bin/valgrind CACHE FILEPATH 
    "Command to use for memory checking xdm" )

# add the unit test include directory
include_directories( third_party/gtest/include )

add_subdirectory( third_party )
add_subdirectory( src )

