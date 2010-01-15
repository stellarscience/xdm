#include <gtest/gtest.h>

#include <mpi.h>

/// Global test environment fixture to enable MPI
class MpiEnvironment : public ::testing::Environment {
private:
  int* mArgc;
  char*** mArgv;
public:
  MpiEnvironment( int* argc, char*** argv ) :
    mArgc( argc ),
    mArgv( argv ) {
  }
  virtual ~MpiEnvironment() {}

  virtual void SetUp() {
    MPI_Init( mArgc, mArgv );
  }

  virtual void TearDown() {
    MPI_Finalize();
  }    
};

/// Test fixture to provide MPI process information to tests that require it.
class MpiTest : public ::testing::Test {
private:
  int mRank;
  int mProcessCount;

public:
  MpiTest() {
    MPI_Comm_size( MPI_COMM_WORLD, &mProcessCount );
    MPI_Comm_rank( MPI_COMM_WORLD, &mRank );
  }

  ~MpiTest() {}

  int processCount() const { return mProcessCount; }
  int rank() const { return mRank; }
};

