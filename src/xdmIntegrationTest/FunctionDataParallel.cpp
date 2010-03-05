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
#define BOOST_TEST_MODULE FunctionDataParallel 
#include <boost/test/unit_test.hpp>

#include <xdm/FileSystem.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmComm/ParallelizeTreeVisitor.hpp>

#include <xdmComm/test/MpiTestFixture.hpp>

#include <plugins/xdmf/TemporalCollection.hpp>

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

  const std::string xmfFile = baseName.str() + ".xmf";
  const std::string hdfFile = baseName.str() + ".h5";

  if ( globalFixture.localRank() == 0 ) {
    xdm::remove( xdm::FileSystemPath( xmfFile ) );
    xdm::remove( xdm::FileSystemPath( hdfFile ) );
  }
  globalFixture.waitAll();
  
  // get the information for the global problem grid
  GridBounds problemBounds = testCaseBounds();

  // construct the problem grid
  ProblemInfo problem = constructFunctionGrid(
    problemBounds, hdfFile );
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
  attribute->dataItem()->setData( xdm::makeRefPtr(
    new FunctionData(
      problemBounds,
      localRegion,
      xdm::makeRefPtr( new TestCaseFunction ) ) ) );

  xdm::RefPtr< xdmf::TimeSeries > timeSeries(
    new xdmf::TemporalCollection( xmfFile, xdm::Dataset::kCreate ) );

  // parallelize, choose a small buffer size to ensure data must be buffered
  // between processes.
  xdmComm::ParallelizeTreeVisitor parallelize( sizeof( double ) );
  grid->accept( parallelize );

  timeSeries->open();
  xdmf::writeTimestepGrid( timeSeries, grid );
  timeSeries->close();
}

} // namespace 

