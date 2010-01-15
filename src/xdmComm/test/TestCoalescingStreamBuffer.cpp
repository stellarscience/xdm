#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmComm/CoalescingStreamBuffer.hpp>

#include <mpi.h>

#include <iostream>

struct CoalescingStreamBuffer : public ::testing::Test {
  xdmComm::CoalescingStreamBuffer buffer;
  int localRank;
  int processes;
  char * result;

  CoalescingStreamBuffer() : 
    buffer( 512, MPI_COMM_WORLD ),
    localRank(),
    processes(0),
    result(0) {
    MPI_Comm_size( MPI_COMM_WORLD, &processes );
    MPI_Comm_rank( MPI_COMM_WORLD, &localRank );
    result = new char[processes];
  }

  ~CoalescingStreamBuffer() {
    delete [] result;
  }

};

BOOST_AUTO_TEST_CASE( sync ) {
  char message = localRank;
  buffer.sputc( message );

  if ( localRank != 0 ) {
    
    buffer.pubsync();
  
  } else {
    
    result[0] = message;

    int received = 1;
    while( received < processes ) {
      buffer.pubsync();
      char recvMessage = buffer.sgetc();
      // order the messages by rank.
      result[recvMessage] = recvMessage;
      received++;
    }

    for ( char i = 0; i < processes; i++ ) {
      BOOST_CHECK_EQUAL( i, result[i] );
    }

  }
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  MPI_Init( &argc, &argv );
  int ret = RUN_ALL_TESTS();
  MPI_Finalize();
  return ret;
}

