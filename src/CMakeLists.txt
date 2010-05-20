
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

# core libraries
add_subdirectory( xdm )
add_subdirectory( xdmGrid )
add_subdirectory( xdmFormat )

# optional libraries
option( XDM_HDF "Build the HDF5 dataset library." ${XDM_HDF} )
if( XDM_HDF )
  add_subdirectory( xdmHdf )
endif()

if ( XDM_COMMUNICATION )
    add_subdirectory( xdmComm )
endif()

add_subdirectory( plugins )

if( BUILD_TESTING AND XDM_COMMUNICATION AND XDM_HDF )
    add_subdirectory( xdmIntegrationTest )
endif()
