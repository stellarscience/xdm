
macro( xdmHdf_serial_test test_name )
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_executable( xdmHdf.${test_name}.test ${ARGN} )
    target_link_libraries( xdmHdf.${test_name}.test xdmHdf ${Boost_LIBRARIES} )
    add_test( xdmHdf.${test_name} xdmHdf.${test_name}.test )
endmacro()

xdmHdf_serial_test( HdfDataset TestHdfDataset.cpp )
xdmHdf_serial_test( SelectionVisitor TestSelectionVisitor.cpp )

