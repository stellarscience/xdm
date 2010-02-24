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
#ifndef xdmComm_test_MpiTestFixture_hpp
#define xdmComm_test_MpiTestFixture_hpp

#include <boost/test/results_reporter.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>

#include <mpi.h>

#include <iostream>

namespace xdmComm {
namespace test {

/// Global fixture for unit tests that must be run in parallel.  This class
/// provides a global test fixture for initializing and finalizing MPI.  In
/// addition, this fixture provides methods for querying the total number of
/// processes and the rank of the local process.  Finally, the fixture redirects
/// logging and output from nonzero ranked processes to stderr.  This means that
/// all critical checks for tests within this fixture should be implemented by
/// rank 0 in the world communicator.
class MpiTestFixture {
private:
  int mLocalRank;
  int mProcesses;

public:

  MpiTestFixture() {
    namespace ut = boost::unit_test;

    MPI_Init( 
      &(boost::unit_test::framework::master_test_suite().argc),
      &(boost::unit_test::framework::master_test_suite().argv) );
    
    MPI_Comm_size( MPI_COMM_WORLD, &mProcesses );
    MPI_Comm_rank( MPI_COMM_WORLD, &mLocalRank );
    
    if ( mLocalRank != 0 ) {
      ut::unit_test_log.set_stream( std::cerr );
      ut::results_reporter::set_stream( std::cerr );
    }
  }

  virtual ~MpiTestFixture() {
    MPI_Finalize();
  }

  int processes() const { return mProcesses; }
  int localRank() const { return mLocalRank; }
  void waitAll() { MPI_Barrier( MPI_COMM_WORLD ); }
};

} // namespace test
} // namespace xdmComm

#endif // xdmComm_test_MpiTestFixture_hpp

