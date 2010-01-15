#include <gtest/gtest.h>

#include <xdmComm/MpiDatasetProxy.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelectionVisitor.hpp>
#include <xdm/HyperslabDataSelection.hpp>

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include <cassert>

class TestSelectionVisitor : public xdm::DataSelectionVisitor {
public:
  size_t startIndex;

  void apply( const xdm::AllDataSelection& s ) {
    startIndex = 0;
  }

  void apply( const xdm::HyperslabDataSelection& s ) {
    assert( s.hyperslab().shape().rank() == 1 );
    startIndex = s.hyperslab().start( 0 );
  }
};

TEST( Local, TestSelectionVisitorApplyHyperslab ) {
  xdm::HyperSlab<> slab( xdm::makeShape( 3 ) );
  slab.setStart( 0, 2 );
  xdm::RefPtr< xdm::DataSelection > selection(
    new xdm::HyperslabDataSelection( slab ) );

  TestSelectionVisitor test;
  selection->accept( test );

  ASSERT_EQ( 2, test.startIndex );
}

class TestDataset : public xdm::Dataset {
public:
  std::vector< int > mValues;

  virtual const char* format() { return "TestDataset"; }
  virtual void writeTextContent( xdm::XmlTextContent& ) {}

  virtual void initializeImplementation( 
    xdm::primitiveType::Value,
    const xdm::DataShape<>& shape )
  {
    mValues.resize( shape[0] );
  }

  virtual void serializeImplementation(
    const xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap )
  {
    TestSelectionVisitor domainVisitor;
    TestSelectionVisitor rangeVisitor;

    selectionMap.domain()->accept( domainVisitor );
    selectionMap.range()->accept( rangeVisitor );

    size_t domainIndex = domainVisitor.startIndex;
    size_t rangeIndex = rangeVisitor.startIndex;

    const int* inputArray = reinterpret_cast< const int* >( data->data() );

    mValues[rangeIndex] = inputArray[domainIndex];
  }

  virtual void finalizeImplementation() {}
};

TEST( MpiDatasetProxy, mpi ) {
  int processes;
  MPI_Comm_size( MPI_COMM_WORLD, &processes );
  int rank;
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  
  std::vector< int > result( processes ); 
  MPI_Allgather( 
    &rank,
    1,
    MPI_INT,
    &result[0],
    1,
    MPI_INT,
    MPI_COMM_WORLD );
  for ( int i = 0; i < processes; i++ ) {
    ASSERT_EQ( i, result[i] );
  }
}

TEST( MpiDatasetProxy, coalesce ) {
  // get process info
  int processes;
  MPI_Comm_size( MPI_COMM_WORLD, &processes );
  int rank;
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  // create a test dataset with an MpiDatasetProxy wrapped around it to handle
  // communication
  xdm::RefPtr< TestDataset > testDataset( new TestDataset );
  xdm::RefPtr< xdm::Dataset > dataset( new xdmComm::MpiDatasetProxy( 
    MPI_COMM_WORLD, testDataset.get(), 1024 ) );

  // set up the data selection for the local process.  We select the hyperslab
  // consisting of one element which starts at the global array location
  // corresponding to my rank.
  xdm::DataShape<> shape = xdm::makeShape( processes );
  xdm::HyperSlab<> slab( shape );
  slab.setStart( 0, rank );
  slab.setStride( 0, 1 );
  slab.setCount( 0, 1 );
  xdm::RefPtr< xdm::DataSelection > localSelection( 
    new xdm::HyperslabDataSelection( slab ) );
  // define the selection map to select all the input data (for this test case a
  // single integer) and map it to the rank'th location in the global structure
  xdm::DataSelectionMap map( new xdm::AllDataSelection, localSelection.get() );

  // the local structured array consists of the pointer to the local rank
  // variable
  xdm::StructuredArray array( xdm::primitiveType::kInt, &rank, 
    xdm::makeShape( 1 ) );

  dataset->initialize( xdm::primitiveType::kInt, shape );
  dataset->serialize( &array, map );
  dataset->finalize();

  if ( rank == 0 ) {
    // check to make sure we wrote all of the data to the array
    ASSERT_EQ( processes, testDataset->mValues.size() );
    for ( int i = 0; i < processes; i++ ) {
      ASSERT_EQ( i, testDataset->mValues[i] );
    }
  } else {
    // the dataset's array should have nothing in it
    ASSERT_EQ( 0, testDataset->mValues.size() );
  }
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  MPI_Init( &argc, &argv );
  int ret = RUN_ALL_TESTS();
  MPI_Finalize();
  return ret;
}

