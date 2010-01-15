cmake_minimum_required( VERSION 2.6 )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules )

include( CTest )
enable_testing()

# option specifies whether or not to build the XDM unit test suite
option( UNIT_TEST_XDM "Enable unit tests for the XDM library" ${UNIT_TEST_XDM} )

# option specifies whether or not to use the XDM communication library
option( XDM_COMMUNICATION 
    "Enable the build of the XDM parallel communication library"
    ${XDM_COMMUNICATION}
)

if( UNIT_TEST_XDM )
    # add the unit test include directory
    include_directories( third_party/gtest/include )
endif()

# Set the compiler flags required for a coverage build
set( CMAKE_C_FLAGS_COVERAGE "-g -O0 -fprofile-arcs -ftest-coverage" )
set( CMAKE_CXX_FLAGS_COVERAGE "-g -O0 -fprofile-arcs -ftest-coverage" )
set( CMAKE_LD_FLAGS_COVERAGE "-g -O0 -fprofile-arcs -ftest-coverage" )

add_subdirectory( third_party )
add_subdirectory( src )

add_subdirectory( doc )

