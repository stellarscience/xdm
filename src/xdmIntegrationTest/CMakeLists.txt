find_package( HDF5 )
find_package( MPI )

include_directories( ${Boost_INCLUDE_DIRS} )

set( XDM_COMPONENTS
    xdm
    xdmComm
    xdmFormat
    xdmGrid
    xdmHdf
)

add_library( xdmIntegrationTest STATIC 
    FunctionData.hpp
    FunctionData.cpp
)

target_link_libraries( xdmIntegrationTest
    ${XDM_COMPONENTS}
)

# add a serial test
macro( xdm_integration_test_serial test_name )
    add_executable( xdmIntegrationTest.${test_name}.test ${ARGN} )
    target_link_libraries( xdmIntegrationTest.${test_name}.test 
        xdmIntegrationTest
        ${XDM_COMPONENTS}
        ${Boost_LIBRARIES} 
    )
    add_test( xdmIntegrationTest.${test_name} xdmIntegrationTest.${test_name}.test )
endmacro()

# compile a parallel executable
macro( xdm_integration_executable_parallel executable_name )
    if( MPI_FOUND )
        include_directories( ${MPI_INCLUDE_PATH} )
        add_executable( xdmIntegrationTest.${executable_name}.test ${ARGN} )
        target_link_libraries( xdmIntegrationTest.${executable_name}.test
            xdmIntegrationTest
            ${XDM_COMPONENTS}
            ${Boost_LIBRARIES} 
            ${MPI_LIBRARIES} 
        )
    endif()
endmacro()

# run a test in parallel with the given number of processes
macro( xdm_integration_run_parallel_test test_name executable_name num_procs )
    if( MPIEXEC )
        add_test( xdmIntegrationTest.${test_name} ${MPIEXEC}
            ${MPIEXEC_NUMPROC_FLAG} ${num_procs}
            ${MPIEXEC_PREFLAGS}
            ${executable_name}
            ${MPIEXEC_POSTFLAGS} 
        )
    endif()
endmacro()

# add a parallel test given a list of source files and the number of processes
macro( xdm_integration_test_parallel test_name num_procs )
    xdm_integration_executable_parallel( ${test_name} ${ARGN} )
    xdm_integration_run_parallel_test( ${test_name}
        xdmIntegrationTest.${test_name}.test ${num_procs} )
endmacro()

# add a test that compares the output of two HDF files.  The test will be run
# only if the MPI executor and h5diff are both available.
macro( xdm_integration_test_hdf5_diff lhs rhs )
    if( MPIEXEC AND HDF5_DIFF_EXECUTABLE )
        add_test( xdmIntegrationTest.diff.${lhs}-to-${rhs} 
            ${HDF5_DIFF_EXECUTABLE} ${lhs} ${rhs}
        )
    endif()
endmacro()

#------------------------------------------------------------------------------
# HdfDatasetMpi Test Suite 
#------------------------------------------------------------------------------
# Build the executable
xdm_integration_executable_parallel( HdfDatasetMpi HdfDatasetMpi.cpp )
# in serial
xdm_integration_run_parallel_test( HdfDataMpi-1 xdmIntegrationTest.HdfDatasetMpi.test 1 )
# Two processes
xdm_integration_run_parallel_test( HdfDataMpi-2 xdmIntegrationTest.HdfDatasetMpi.test 2 )
xdm_integration_test_hdf5_diff( HdfDataMpi-1.h5 HdfDataMpi-2.h5 )
# Four processes
xdm_integration_run_parallel_test( HdfDataMpi-4 xdmIntegrationTest.HdfDatasetMpi.test 4 )
xdm_integration_test_hdf5_diff( HdfDataMpi-1.h5 HdfDataMpi-4.h5 )
# Eight processes
xdm_integration_run_parallel_test( HdfDataMpi-8 xdmIntegrationTest.HdfDatasetMpi.test 8 )
xdm_integration_test_hdf5_diff( HdfDataMpi-1.h5 HdfDataMpi-8.h5 )

#------------------------------------------------------------------------------
# FunctionData Test Suite 
#------------------------------------------------------------------------------
# run it in serial
xdm_integration_test_serial( FunctionData.serial FunctionDataSerial.cpp ) 
xdm_integration_executable_parallel( 
    FunctionData.parallel
    FunctionDataParallel.cpp
)
# run it with 2 processes
xdm_integration_run_parallel_test( 
    FunctionData.parallel.2
    xdmIntegrationTest.FunctionData.parallel.test
    2
)
# check the parallel and serial files are equal
xdm_integration_test_hdf5_diff( 
    FunctionData.serial.h5
    FunctionData.parallel.2.h5 
)
# run it with 4 processes
xdm_integration_run_parallel_test( 
    FunctionData.parallel.4
    xdmIntegrationTest.FunctionData.parallel.test
    4
)
# check the parallel and serial files are equal
xdm_integration_test_hdf5_diff( 
    FunctionData.serial.h5
    FunctionData.parallel.4.h5 
)
# run it with 8 processes
xdm_integration_run_parallel_test( 
    FunctionData.parallel.8
    xdmIntegrationTest.FunctionData.parallel.test
    8
)
# check the parallel and serial files are equal
xdm_integration_test_hdf5_diff( 
    FunctionData.serial.h5
    FunctionData.parallel.8.h5 
)

