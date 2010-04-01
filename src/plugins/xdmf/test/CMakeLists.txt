
macro( xdmfPlugin_serial_test test_name )
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_executable( xdmFormat.${test_name}.test ${ARGN} )
    target_link_libraries( xdmFormat.${test_name}.test 
      ${Boost_LIBRARIES} 
      xdmXdmfPlugin )
    add_test( xdmXdmfPlugin.${test_name} xdmFormat.${test_name}.test )
endmacro()

xdmfPlugin_serial_test( XdmfGridCompatibility TestXdmfGridCompatibility.cpp )
xdmfPlugin_serial_test( XmfReader TestXmfReader.cpp )

