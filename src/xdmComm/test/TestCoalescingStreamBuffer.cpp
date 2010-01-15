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

} // namespace

