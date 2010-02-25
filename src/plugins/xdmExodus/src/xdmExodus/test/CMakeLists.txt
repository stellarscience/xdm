
# Add a serial test
macro( xdmExodus_test_serial test_name )
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_executable( xdmExodus.${test_name}.test ${ARGN} )
    target_link_libraries( xdmExodus.${test_name}.test xdmExodus ${Boost_LIBRARIES} )
    add_test( xdmExodus.${test_name} xdmExodus.${test_name}.test )
endmacro()

xdmExodus_test_serial( ExodusReader TestExodusReader.cpp )

