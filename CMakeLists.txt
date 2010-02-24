#
# XDM: The eXtensible Data Model (based on the XDMF format)
#

cmake_minimum_required( VERSION 2.6 )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules )

# option specifies whether or not to use the XDM communication library
option( XDM_COMMUNICATION 
    "Enable the build of the XDM parallel communication library"
    ${XDM_COMMUNICATION}
)

# option specifies whether or not to enable the unit tests
option( BUILD_TESTING 
  "Enable the build of the project's unit tests."
  OFF 
)

# option to build plugins
option( BUILD_PLUGINS
  "Enable the build of the plugins directory."
  OFF
)

if( BUILD_TESTING )
  message( STATUS "Unit tests are enabled" )
  include( CTest )
  
  # Use Boost for testing. Currently, the test executables require a static
  # link to the Boost unit test framework library.
  set( Boost_USE_STATIC_LIBS ON CACHE BOOL 
    "Prefer a static link to the Boost libraries." )
  mark_as_advanced( Boost_USE_STATIC_LIBS )
  find_package( Boost REQUIRED COMPONENTS unit_test_framework )
endif()

if ( NOT CMAKE_BUILD_TYPE )
  message( STATUS "No build type specified -- default is Release" )
  set( CMAKE_BUILD_TYPE Release CACHE STRING "Default build type" FORCE )
endif()

# Set the compiler flags required for a coverage build
if ( CMAKE_COMPILER_IS_GNUC OR CMAKE_COMPILER_IS_GNUCXX )
  set( CMAKE_C_FLAGS_COVERAGE "-g -O0 -fno-default-inline -fprofile-arcs -ftest-coverage" )
  set( CMAKE_CXX_FLAGS_COVERAGE "-g -O0 -fno-default-inline -fprofile-arcs -ftest-coverage" )
  set( CMAKE_LD_FLAGS_COVERAGE "-g -O0 -fno-default-inline -fprofile-arcs -ftest-coverage" )
endif()

add_subdirectory( src )

add_subdirectory( doc )

