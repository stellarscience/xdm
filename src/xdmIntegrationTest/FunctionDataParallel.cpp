#include <gtest/gtest.h>

#include <xdmIntegrationTest/ParallelTestHelpers.hpp>

#include <xdm/HyperSlab.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmComm/ParallelizeTreeVisitor.hpp>

#include <xdmFormat/TemporalCollection.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Grid.hpp>

#include <xdmIntegrationTest/FunctionData.hpp>

#include <mpi.h>

#include <algorithm>
#include <sstream>

class FunctionDataParallel : public MpiTest {};

TEST_F( FunctionDataParallel, writeResult ) {
  std::stringstream baseName;
  baseName << "FunctionData.parallel." << processCount();
  
  // get the information for the global problem grid
  GridBounds problemBounds = testCaseBounds();

  // construct the problem grid
  ProblemInfo problem = constructFunctionGrid(
    problemBounds, baseName.str() + ".h5" );
  xdm::RefPtr< xdmGrid::Grid > grid = problem.first;
  xdm::RefPtr< xdmGrid::Attribute > attribute = problem.second;

  // To compute the hyperslab of interest for this process, we'll partition the
  // grid along the x axis and assign roughly equal volumes to everyone.
  int planesPerProcess = problemBounds.size(0) / processCount();
  int remainingPlanes = problemBounds.size(0) % processCount();
  
  // distribute the remaining number of planes among the first few processes
  int localNumberOfPlanes = planesPerProcess;
  int localStartingPlane = rank() * planesPerProcess;
  if ( rank() < remainingPlanes ) {
    localNumberOfPlanes += 1;
    localStartingPlane += rank();
  } else {
    localStartingPlane += remainingPlanes;
  }

  xdm::HyperSlab<> localRegion( problemBounds.shape() );
  localRegion.setStart( 0, localStartingPlane );
  localRegion.setStart( 1, 0 );
  localRegion.setStart( 2, 0 );
  std::fill( localRegion.beginStride(), localRegion.endStride(), 1 );
  localRegion.setCount( 0, localNumberOfPlanes );
  localRegion.setCount( 1, problemBounds.size(1) );
  localRegion.setCount( 2, problemBounds.size(2) );

  // add the data for the region of interest to the grid attribute
  attribute->dataItem()->appendData( new FunctionData( 
    problemBounds, 
    localRegion, 
    new TestCaseFunction ) );

  xdm::RefPtr< xdmFormat::TimeSeries > timeSeries(
    new xdmFormat::TemporalCollection( baseName.str() ) );

  xdmComm::ParallelizeTreeVisitor parallelize( 
    localRegion.size() * sizeof( double ) + 1024 );
  grid->accept( parallelize );

  timeSeries->open();
  xdmFormat::writeTimestepGrid( timeSeries, grid );
  timeSeries->close();
}

int main( int argc, char* argv[] ) {
  ::testing::AddGlobalTestEnvironment( new MpiEnvironment( &argc, &argv ) );
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

