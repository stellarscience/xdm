//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.      
//                                                                             
// This file is part of XDM                                                    
//                                                                             
// This program is free software: you can redistribute it and/or modify it     
// under the terms of the GNU Lesser General Public License as published by    
// the Free Software Foundation, either version 3 of the License, or (at your  
// option) any later version.                                                  
//                                                                             
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        
// License for more details.                                                   
//                                                                             
// You should have received a copy of the GNU Lesser General Public License    
// along with this program.  If not, see <http://www.gnu.org/licenses/>.       
//                                                                             
//------------------------------------------------------------------------------
#define BOOST_TEST_MODULE CoalescingStreamBuffer
#include <boost/test/unit_test.hpp>
#include <boost/test/results_reporter.hpp>
#include <boost/test/unit_test_log.hpp>

#include <xdmComm/BarrierOnExit.hpp>
#include <xdmComm/CoalescingStreamBuffer.hpp>

#include <xdmComm/test/MpiTestFixture.hpp>

#include <mpi.h>

#include <algorithm>
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
    std::fill( result, result + globalFixture.processes(), 0 );
  }

  ~Fixture() {
    delete [] result;
  }

};

BOOST_AUTO_TEST_CASE( synchronize ) {
  Fixture test;
  xdmComm::BarrierOnExit barrier( MPI_COMM_WORLD );

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
      while ( test.buffer.poll() ) {
        test.buffer.pubsync();
        char recvMessage = test.buffer.sbumpc();
        // order the messages by rank.
        test.result[recvMessage] = recvMessage;
        received++;
      }
    }

    for ( char i = 0; i < globalFixture.processes(); i++ ) {
      BOOST_CHECK_EQUAL( i, test.result[i] );
    }

  }
}

BOOST_AUTO_TEST_CASE( overflow ) {
  xdmComm::BarrierOnExit barrier( MPI_COMM_WORLD );

  // Test with a 3 byte buffer to make sure the messages don't align nicely.
  xdmComm::CoalescingStreamBuffer test( 3, MPI_COMM_WORLD );
  std::vector< int > result( globalFixture.processes() * 4, 0 );

  // each process will send an array of ints with 4 copies of it's rank
  int message[4];
  std::fill( message, message + 4, globalFixture.localRank() );

  if ( globalFixture.localRank() != 0 ) {
    test.sputn( reinterpret_cast< char* >( message ), sizeof( int ) * 4 );
    test.pubsync();
  } else {
    std::copy( message, message + 4, result.begin() );

    int received = 1;
    while ( received < globalFixture.processes() ) {
      while ( test.poll() ) {
        test.pubsync();
        test.sgetn( reinterpret_cast< char* >( message ), sizeof( int ) * 4 );
        int source = test.currentSource();
        std::copy( message, message + 4, result.begin() + 4 * source );
        received++;
      }
    }

    std::vector< int > answer( globalFixture.processes() * 4 );
    for ( int i = 0; i < globalFixture.processes(); i++ ) {
      for ( int j = 4*i; j < 4*i + 4; j++ ) {
        answer[j] = i;
      }
    }
    BOOST_CHECK_EQUAL_COLLECTIONS( answer.begin(), answer.end(),
      result.begin(), result.end() );

  }
}

} // namespace

