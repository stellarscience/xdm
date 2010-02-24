#
# Exodus II plugin for XDM
#
cmake_minimum_required(VERSION 2.6)

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules )

find_package( NetCDF REQUIRED )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/exodusii/cbind/include
  ${CMAKE_CURRENT_BINARY_DIR}/exodusii/cbind/include
  ${NETCDF_INCLUDE_DIR}
)

if( BUILD_TESTING )
  enable_testing()
endif()

add_subdirectory( exodusii )
add_subdirectory( src )
