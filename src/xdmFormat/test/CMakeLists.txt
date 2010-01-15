
macro( xdmFormat_serial_test test_name )
    add_executable( xdmFormat.${test_name}.test ${ARGN} )
    target_link_libraries( xdmFormat.${test_name}.test gtest xdmFormat )
    add_test( xdmFormat.${test_name} xdmFormat.${test_name}.test )
endmacro()

xdmFormat_serial_test( XdmfGridCompatibility TestXdmfGridCompatibility.cpp )

