
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

# core libraries
add_subdirectory( xdm )
add_subdirectory( xdmGrid )
add_subdirectory( xdmHdf )
add_subdirectory( xdmFormat )

# optional libraries
if ( XDM_COMMUNICATION )
    add_subdirectory( xdmComm )
endif()

if( UNIT_TEST_XDM AND XDM_COMMUNICATION )
    add_subdirectory( xdmIntegrationTest )
endif()
