#define BOOST_TEST_MODULE FunctionDataParallel 
#include <boost/test/unit_test.hpp>

#include <xdm/HyperSlab.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmComm/ParallelizeTreeVisitor.hpp>

#include <xdmComm/test/MpiTestFixture.hpp>

#include <xdmFormat/TemporalCollection.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Grid.hpp>

#include <xdmIntegrationTest/FunctionData.hpp>

#include <mpi.h>

#include <algorithm>
#include <sstream>

namespace {

xdmComm::test::MpiTestFixture globalFixture;

BOOST_AUTO_TEST_CASE( writeResult ) {
  std::stringstream baseName;
  baseName << "FunctionData.parallel." << globalFixture.processes();
  
  // get the information for the global problem grid
  GridBounds problemBounds = testCaseBounds();

  // construct the problem grid
  ProblemInfo problem = constructFunctionGrid(
    problemBounds, baseName.str() + ".h5" );
  xdm::RefPtr< xdmGrid::Grid > grid = problem.first;
  xdm::RefPtr< xdmGrid::Attribute > attribute = problem.second;

  // To compute the hyperslab of interest for this process, we'll partition the
  // grid along the x axis and assign roughly equal volumes to everyone.
  int planesPerProcess = problemBounds.size(0) / globalFixture.processes();
  int remainingPlanes = problemBounds.size(0) % globalFixture.processes();
  
  // distribute the remaining number of planes among the first few processes
  int localNumberOfPlanes = planesPerProcess;
  int localStartingPlane = globalFixture.localRank() * planesPerProcess;
  if ( globalFixture.localRank() < remainingPlanes ) {
    localNumberOfPlanes += 1;
    localStartingPlane += globalFixture.localRank();
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

} // namespace 

