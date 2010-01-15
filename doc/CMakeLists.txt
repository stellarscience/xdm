project( XdmDocumentation )

#
# Project to build the documentation for the library.
#

find_package( Doxygen )

# Configure the doxygen input file to get the correct paths
configure_file( 
  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  @ONLY
)

if( DOXYGEN_FOUND )

  add_custom_target( doc 
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  )

endif()

