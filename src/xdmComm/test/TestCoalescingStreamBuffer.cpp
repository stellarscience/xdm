#define BOOST_TEST_MODULE CoalescingStreamBuffer
#include <boost/test/unit_test.hpp>
#include <boost/test/results_reporter.hpp>
#include <boost/test/unit_test_log.hpp>

#include <xdmComm/CoalescingStreamBuffer.hpp>

#include <xdmComm/test/MpiTestFixture.hpp>

#include <mpi.h>

#include <iostream>
#include <sstream>

namespace {

xdmComm::test::MpiTestFixture globalFixture;

struct Fixture {
  xdmComm::CoalescingStreamBuffer buffer;
  char * result;

  Fixture() : 
    buffer( 512, MPI_COMM_WORLD ),
    result(0) 
  {
    result = new char[globalFixture.processes()];
  }

  ~Fixture() {
    delete [] result;
  }

};

BOOST_AUTO_TEST_CASE( synchronize ) {
  Fixture test;

  char message = globalFixture.localRank();
  test.buffer.sputc( message );

  if ( globalFixture.localRank() != 0 ) {
    
    // ranks != 0 send a single character with their rank
    test.buffer.pubsync();
  
  } else {
    
    // write rank 0 to the first location of the result buffer
    test.result[0] = message;

    // receive a single number from all other processes and write them in order
    // of rank to the result buffer.
    int received = 1;
    while( received < globalFixture.processes() ) {
      test.buffer.pubsync();
      char recvMessage = test.buffer.sgetc();
      // order the messages by rank.
      test.result[recvMessage] = recvMessage;
      received++;
    }

    for ( char i = 0; i < globalFixture.processes(); i++ ) {
      BOOST_CHECK_EQUAL( i, test.result[i] );
    }

  }
}

} // namespace anon
