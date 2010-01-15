#include <gtest/gtest.h>

#include <xdm/HyperSlab.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmFormat/TemporalCollection.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Grid.hpp>

#include <xdmIntegrationTest/FunctionData.hpp>

#include <algorithm>

TEST( FunctionDataSerial, writeResult ) {
  // get the information for the problem grid
  GridBounds problemBounds = testCaseBounds();

  // construct the problem grid
  ProblemInfo problem = constructFunctionGrid( 
    testCaseBounds(),
    "FunctionData.serial.h5" );

  // in serial, the hyperslab of interest is the whole problem
  xdm::HyperSlab<> slab( problemBounds.shape() );
  std::fill( slab.beginStart(), slab.endStart(), 0 );
  std::fill( slab.beginStride(), slab.endStride(), 1 );
  std::copy( 
    problemBounds.shape().begin(), 
    problemBounds.shape().end(),
    slab.beginCount() );

  // add the data for the slab of interest to the grid attribute
  xdm::RefPtr< xdmGrid::Attribute > attribute = problem.second;
  attribute->dataItem()->appendData( new FunctionData( 
    problemBounds, 
    slab, 
    new TestCaseFunction ) );
  
  xdm::RefPtr< xdmFormat::TimeSeries > timeSeries( 
    new xdmFormat::TemporalCollection( "FunctionData.serial" ) );
  
  timeSeries->open();
  xdmFormat::writeTimestepGrid( timeSeries, problem.first );
  timeSeries->close();
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

