
macro( xdmfPlugin_serial_test test_name )
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_executable( xdmFormat.${test_name}.test ${ARGN} )
    target_link_libraries( xdmFormat.${test_name}.test 
      ${Boost_LIBRARIES} 
      xdmXdmfPlugin )
    add_test( xdmXdmfPlugin.${test_name} xdmFormat.${test_name}.test )
endmacro()

macro( copy_test_file _file )
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${_file} ${CMAKE_CURRENT_BINARY_DIR}/${_file} 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_file}
  )
  add_custom_target( copy_${_file} ALL 
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_file} )
endmacro()

copy_test_file( test_document1.xmf )
copy_test_file( test_document2.xmf )

xdmfPlugin_serial_test( XdmfGridCompatibility TestXdmfGridCompatibility.cpp )
xdmfPlugin_serial_test( XmfReader TestXmfReader.cpp )
xdmfPlugin_serial_test( ImplTreeBuilder TestImplTreeBuilder.cpp )

