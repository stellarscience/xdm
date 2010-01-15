//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
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

};

} // namespace test
} // namespace xdmComm

#endif // xdmComm_test_MpiTestFixture_hpp

