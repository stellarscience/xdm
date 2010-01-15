
macro( xdmGrid_test_serial test_name )
    add_executable( ${test_name} ${ARGN} )
    target_link_libraries( ${test_name} xdmGrid gtest )
    add_test( ${test_name} ${test_name} )
endmacro()

xdmGrid_test_serial( Grid Grid.cpp )
xdmGrid_test_serial( UniformGrid UniformGrid.cpp )

