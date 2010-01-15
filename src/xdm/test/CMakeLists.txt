
# Add a serial test
macro( xdm_test_serial test_name )
    add_executable( xdm.${test_name}.test ${ARGN} )
    target_link_libraries( xdm.${test_name}.test xdm gtest )
    add_test( xdm.${test_name} xdm.${test_name}.test )
endmacro()

# add a parallel test
macro( xdm_test_parallel test_name num_procs )
    find_package( MPI )
    if( MPI_FOUND )
        include_directories( ${MPI_INCLUDE_PATH} )
        add_executable( ${test_name} ${ARGN} )
        target_link_libraries( ${test_name} xdm ${MPI_LIBRARIES} )
        add_test( ${test_name} ${MPIEXEC}
            ${MPIEXEC_NUMPROC_FLAG} ${num_procs}
            ${MPIEXEC_PREFLAGS}
            ${test_name}
            ${MPIEXEC_POSTFLAGS} )
    endif( MPI_FOUND )
endmacro()

xdm_test_serial( HdfDatasetSerial HdfDataset.cpp )
xdm_test_serial( TemplateStructuredArray TemplateStructuredArray.cpp )
xdm_test_serial( XmlObject XmlObject.cpp )
xdm_test_serial( XmlOutputStream XmlOutputStream.cpp )
xdm_test_serial( Item Item.cpp )
xdm_test_serial( DataShape DataShape.cpp )
xdm_test_serial( ThrowMacro ThrowMacro.cpp )
xdm_test_serial( UniformDataItem UniformDataItem.cpp )
xdm_test_serial( CompositeDataItem CompositeDataItem.cpp )
xdm_test_serial( CollectMetadataOperation CollectMetadataOperation.cpp )
xdm_test_serial( RefPtr RefPtr.cpp )

# xdm_test_parallel( HdfDatasetMPI 4 HdfDatasetMPI.cpp )

