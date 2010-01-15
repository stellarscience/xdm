cmake_minimum_required( VERSION 2.6 )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules )

enable_testing()

add_subdirectory( src )

