cmake_minimum_required( VERSION 2.6 )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules )

enable_testing()

option( UNIT_TEST_XDM "Enable unit tests for the XDM library" ${UNIT_TEST_XDM} )

if( UNIT_TEST_XDM )
    # add the unit test include directory
    include_directories( third_party/gtest/include )
endif()

add_subdirectory( third_party )
add_subdirectory( src )

