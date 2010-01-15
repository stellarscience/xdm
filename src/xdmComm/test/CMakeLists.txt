# add a serial test
macro( xdmComm_test_serial test_name )
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_executable( xdmComm.${test_name}.test ${ARGN} )
    target_link_libraries( xdmComm.${test_name}.test xdmComm ${Boost_LIBRARIES} )
    add_test( xdmComm.${test_name} xdmComm.${test_name}.test )
endmacro()

# add a parallel test
macro( xdmComm_test_parallel test_name num_procs )
    find_package( MPI )
    if( MPI_FOUND AND MPIEXEC )
        include_directories( 
          ${Boost_INCLUDE_DIRS}
          ${MPI_INCLUDE_PATH} )
        add_executable( xdmComm.${test_name}.test ${ARGN} )
        target_link_libraries( xdmComm.${test_name}.test
            xdmComm 
            ${Boost_LIBRARIES} 
            ${MPI_LIBRARIES} )
        add_test( xdmComm.${test_name} ${MPIEXEC}
            ${MPIEXEC_NUMPROC_FLAG} ${num_procs}
            ${MPIEXEC_PREFLAGS}
            xdmComm.${test_name}.test
            ${MPIEXEC_POSTFLAGS} )
    endif( MPI_FOUND AND MPIEXEC )
endmacro()

xdmComm_test_serial( BinaryStreamBuffer TestBinaryStreamBuffer.cpp )
xdmComm_test_serial( BinaryOStream TestBinaryOStream.cpp )
xdmComm_test_serial( BinaryIStream TestBinaryIStream.cpp )
xdmComm_test_serial( BinaryIOStream TestBinaryIOStream.cpp )
xdmComm_test_serial( BinaryStreamOperations TestBinaryStreamOperations.cpp )

xdmComm_test_parallel( MpiDatasetProxy 4 TestMpiDatasetProxy.cpp )
xdmComm_test_parallel( CoalescingStreamBuffer 4 TestCoalescingStreamBuffer.cpp )

