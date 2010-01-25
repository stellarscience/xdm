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
#define BOOST_TEST_MODULE FunctionDataSerial 
#include <boost/test/unit_test.hpp>

#include <xdm/HyperSlab.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmFormat/TemporalCollection.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Grid.hpp>

#include <xdmIntegrationTest/FunctionData.hpp>

#include <algorithm>

namespace {

BOOST_AUTO_TEST_CASE( writeResult ) {
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
  attribute->dataItem()->appendData( xdm::makeRefPtr( new FunctionData(
    problemBounds, 
    slab, 
    xdm::makeRefPtr( new TestCaseFunction ) ) ) );
  
  xdm::RefPtr< xdmFormat::TimeSeries > timeSeries( 
    new xdmFormat::TemporalCollection( "FunctionData.serial.xmf" ) );
  
  timeSeries->open();
  xdmFormat::writeTimestepGrid( timeSeries, problem.first );
  timeSeries->close();
}

} // namespace 

