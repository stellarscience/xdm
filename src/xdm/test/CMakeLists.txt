
# serial HDF dataset test program
add_executable( HdfDataset HdfDataset.cpp )
target_link_libraries( HdfDataset xdm )
add_test( HdfDataset HdfDataset )

find_package( MPI )
if( MPI_FOUND )
    include_directories( ${MPI_INCLUDE_PATH} )

    # MPI HDF output via serial io and communication test program
    add_executable( HdfDatasetMPI HdfDatasetMPI.cpp )
    target_link_libraries( HdfDatasetMPI xdm ${MPI_LIBRARIES} )
    add_test( HdfDatasetMPI ${MPIEXEC} 
        ${MPIEXEC_NUMPROC_FLAG} 4
        ${MPIEXEC_PREFLAGS}
        HdfDatasetMPI 
        ${MPIEXEC_POSTFLAGS} )

endif( MPI_FOUND )
