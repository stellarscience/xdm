project( XdmDocumentation )

#
# Project to build the documentation for the library.
#

find_package( Doxygen )

# Configure the doxygen input file for html documentation
set( GENERATE_LATEX "NO" )
configure_file( 
  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.html
  @ONLY
)

# Configure the doxygen input file for LaTeX documentation
set( GENERATE_LATEX "YES" )
configure_file( 
  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.latex
  @ONLY
)

if( DOXYGEN_FOUND )

  add_custom_target( htmldoc 
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.html
  )
  
  add_custom_target( latexdoc 
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.latex
  )

endif()

