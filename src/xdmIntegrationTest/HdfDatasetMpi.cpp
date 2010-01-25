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
#define BOOST_TEST_MODULE HdfDatasetMpi 
#include <boost/test/unit_test.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelectionMap.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/TemplateStructuredArray.hpp>

#include <xdmComm/MpiDatasetProxy.hpp>

#include <xdmComm/test/MpiTestFixture.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <mpi.h>

#include <sstream>
#include <vector>

namespace {

static const int kSize = 100;

xdmComm::test::MpiTestFixture globalFixture;

BOOST_AUTO_TEST_CASE( writeDataset1D ) {
  std::stringstream testCaseFile;
  testCaseFile << "HdfDataMpi-" << globalFixture.processes() << ".h5";
  
  xdm::RefPtr< xdmHdf::HdfDataset > hdfDataset( new xdmHdf::HdfDataset );
  hdfDataset->setFile( testCaseFile.str() );
  hdfDataset->setDataset( "Values" );

  xdm::DataShape<> shape = xdm::makeShape( kSize );

  int valuesPerProcess = kSize / globalFixture.processes();
  int remainder = kSize % globalFixture.processes();

  // divide up the remainder so that all spaces are filled by someone
  int localNumberOfValues = valuesPerProcess;
  int localStart = globalFixture.localRank() * valuesPerProcess;
  if ( globalFixture.localRank() < remainder ) {
    // add one space to the current process realm
    localNumberOfValues += 1;
    // offset the start location according to the number of spaces added to the
    // processes before this one, in this case it is the rank since exactly one
    // space was added to all previous ranks
    localStart += globalFixture.localRank();
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
      processData.size() ) );

  xdm::DataSelectionMap selectionMap(
    xdm::makeRefPtr( new xdm::AllDataSelection ),
    xdm::makeRefPtr( new xdm::HyperslabDataSelection( processSlab ) ) );

  xdm::RefPtr< xdm::Dataset > dataset( 
    new xdmComm::MpiDatasetProxy( 
      MPI_COMM_WORLD, 
      hdfDataset,
      processData.size() * sizeof(int) + 1024 ) );

  dataset->initialize( xdm::primitiveType::kInt, shape, xdm::Dataset::kCreate );
  dataset->serialize( processArray.get(), selectionMap );
  dataset->finalize();
}

} // namespace 

