
macro( xdmHdf_serial_test test_name )
    add_executable( xdmHdf.${test_name}.test ${ARGN} )
    target_link_libraries( xdmHdf.${test_name}.test xdmHdf gtest )
    add_test( xdmHdf.${test_name} xdmHdf.${test_name}.test )
endmacro()

xdmHdf_serial_test( HdfDataset HdfDataset.cpp )
xdmHdf_serial_test( SelectionVisitor SelectionVisitor.cpp )

