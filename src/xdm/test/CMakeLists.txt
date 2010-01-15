
# Add a serial test
macro( xdm_test_serial test_name )
    add_executable( ${test_name} ${ARGN} )
    target_link_libraries( ${test_name} xdm gtest )
    add_test( ${test_name} ${test_name} )
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

# xdm_test_parallel( HdfDatasetMPI 4 HdfDatasetMPI.cpp )

