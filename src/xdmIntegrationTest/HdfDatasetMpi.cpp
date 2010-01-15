#include <gtest/gtest.h>

#include <xdmIntegrationTest/ParallelTestHelpers.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelectionMap.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/TemplateStructuredArray.hpp>

#include <xdmComm/MpiDatasetProxy.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <mpi.h>

#include <sstream>
#include <vector>

static const int kSize = 100;

class HdfDatasetMpi : public MpiTest {};

TEST_F( MpiTest, writeDataset1D ) {
  std::stringstream testCaseFile;
  testCaseFile << "HdfDataMpi-" << processCount() << ".h5";
  
  xdm::RefPtr< xdmHdf::HdfDataset > hdfDataset( new xdmHdf::HdfDataset );
  hdfDataset->setFile( testCaseFile.str() );
  hdfDataset->setDataset( "Values" );

  xdm::DataShape<> shape = xdm::makeShape( kSize );

  int valuesPerProcess = kSize / processCount();
  int remainder = kSize % processCount();

  // divide up the remainder so that all spaces are filled by someone
  int localNumberOfValues = valuesPerProcess;
  int localStart = rank() * valuesPerProcess;
  if ( rank() < remainder ) {
    // add one space to the current process realm
    localNumberOfValues += 1;
    // offset the start location according to the number of spaces added to the
    // processes before this one, in this case it is the rank since exactly one
    // space was added to all previous ranks
    localStart += rank();
  } else {
    // offset the start location by the number of spaces added to the processes
    // with lower rank than the local process, in this case, it is just the
    // remainder. 
    localStart += remainder;
  }

  xdm::HyperSlab<> processSlab( shape );
  processSlab.setStart( 0, localStart );
  processSlab.setStride( 0, 1 );
  processSlab.setCount( 0, localNumberOfValues );

  std::vector< int > processData( processSlab.count(0) );

  for ( int i = 0; i < processData.size(); i++ ) {
    processData[i] = processSlab.start(0) + i;
  }
  xdm::RefPtr< xdm::StructuredArray > processArray( 
    xdm::createStructuredArray( 
      &processData[0], 
      xdm::makeShape( processData.size() ) ) );

  xdm::DataSelectionMap selectionMap(
    new xdm::AllDataSelection,
    new xdm::HyperslabDataSelection( processSlab ) );

  xdm::RefPtr< xdm::Dataset > dataset( 
    new xdmComm::MpiDatasetProxy( 
      MPI_COMM_WORLD, 
      hdfDataset.get(),
      processData.size() * sizeof(int) + 1024 ) );

  dataset->initialize( xdm::primitiveType::kInt, shape );
  dataset->serialize( processArray, selectionMap );
  dataset->finalize();
}

int main( int argc, char* argv[] ) {
  ::testing::AddGlobalTestEnvironment( new MpiEnvironment( &argc, &argv ) );
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
