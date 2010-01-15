# add a serial test
macro( xdmComm_test_serial test_name )
    add_executable( xdmComm.${test_name}.test ${ARGN} )
    target_link_libraries( xdmComm.${test_name}.test xdmComm gtest )
    add_test( xdmComm.${test_name} xdmComm.${test_name}.test )
endmacro()

# add a parallel test
macro( xdmComm_test_parallel test_name num_procs )
    find_package( MPI )
    if( MPI_FOUND )
        include_directories( ${MPI_INCLUDE_PATH} )
        add_executable( xdmComm.${test_name}.test ${ARGN} )
        target_link_libraries( xdmComm.${test_name}.test
            xdmComm 
            gtest 
            ${MPI_LIBRARIES} )
        add_test( xdmComm.${test_name} ${MPIEXEC}
            ${MPIEXEC_NUMPROC_FLAG} ${num_procs}
            ${MPIEXEC_PREFLAGS}
            xdmComm.${test_name}.test
            ${MPIEXEC_POSTFLAGS} )
    endif( MPI_FOUND )
endmacro()

xdmComm_test_serial( BinaryStreamBuffer BinaryStreamBuffer.cpp )
xdmComm_test_serial( BinaryOStream BinaryOStream.cpp )
xdmComm_test_serial( BinaryIStream BinaryIStream.cpp )
xdmComm_test_serial( BinaryIOStream BinaryIOStream.cpp )

xdmComm_test_parallel( MpiDatasetProxy 4 MpiDatasetProxy.cpp )

