
macro( xdmFormat_serial_test test_name )
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_executable( xdmFormat.${test_name}.test ${ARGN} )
    target_link_libraries( xdmFormat.${test_name}.test ${Boost_LIBRARIES} xdmFormat )
    add_test( xdmFormat.${test_name} xdmFormat.${test_name}.test )
endmacro()

xdmFormat_serial_test( XdmfGridCompatibility TestXdmfGridCompatibility.cpp )

