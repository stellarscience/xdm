
option( EXODUS_PLUGIN "Build the ExodusII plugin for XDM." OFF )
if( EXODUS_PLUGIN )
  add_subdirectory( xdmExodus )
endif()

option( XDMF_PLUGIN "Build the XDMF plugin for XDM." OFF )
if( XDMF_PLUGIN )
  add_subdirectory( xdmf )
endif()

