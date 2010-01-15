#include <gtest/gtest.h>

#include <xdmComm/MpiDatasetProxy.hpp>

#include <mpi.h>

#include <iostream>

struct MpiInit {
  MpiInit( int& argc, char**& argv ) {
    MPI_Init( &argc, &argv );
  }
  ~MpiInit() {
    MPI_Finalize();
  }
};

TEST( MpiDatasetProxy, mpi ) {
  int rank;
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  
  int result[4];
  MPI_Allgather( 
    &rank,
    1,
    MPI_INT,
    result,
    1,
    MPI_INT,
    MPI_COMM_WORLD );
  for ( int i = 0; i < 4; i++ ) {
    ASSERT_EQ( i, result[i] );
  }
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  MPI_Init( &argc, &argv );
  int ret = RUN_ALL_TESTS();
  MPI_Finalize();
  return ret;
}

