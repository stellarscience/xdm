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
#define BOOST_TEST_MODULE TestHdfDataset
#include <boost/test/unit_test.hpp>

#include <xdm/StructuredArray.hpp>
#include <xdm/VectorStructuredArray.hpp>
#include <xdm/RefPtr.hpp>

#include <xdm/DataSelection.hpp>

#include <xdmHdf/FileIdentifierRegistry.hpp>
#include <xdmHdf/HdfDataset.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>

#include <cstdlib>

namespace {

size_t fileSize( const char* filename ) {
  std::ifstream file( filename );
  if ( !file.good() || file.eof() || !file.is_open() ) {
    return 0;
  }
  file.seekg( 0, std::ios_base::beg );
  std::ifstream::pos_type beginpos = file.tellg();
  file.seekg( 0, std::ios_base::end );
  return ( file.tellg() - beginpos );
}

BOOST_AUTO_TEST_CASE( roundtrip ) {
    
  // set up the input data
  xdm::VectorStructuredArray< int > data(16);
  srand( 42 );
  std::generate( data.begin(), data.end(), &rand );
  
  // put the write code in its own scope so we can ensure that the file is
  // actually closed and reopened later.
  {
    
    // initialize the dataset on disk
    xdm::DataShape<> fileshape( 2 );
    fileshape[0] = 4;
    fileshape[1] = 4;

    // create the dataset
    xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset() );
    dataset->setFile( "HdfDataset.h5" );
    dataset->setDataset( "testdata" );

    // write the data to disk
    dataset->initialize( xdm::primitiveType::kInt, fileshape,
      xdm::Dataset::kCreate );
    dataset->serialize( &data, xdm::DataSelectionMap() );
    dataset->finalize();
  
  } // end of writing
  
  // make sure that all files are closed
  xdmHdf::FileIdentifierRegistry::instance()->closeAllIdentifiers();

  xdm::VectorStructuredArray< int > result( 16 );
  { // begin reading

    // specify the expected size of the dataset on disk
    xdm::DataShape<> fileshape( 2 );
    fileshape[0] = 4;
    fileshape[1] = 4;

    // open the dataset
    xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset() );
    dataset->setFile( "HdfDataset.h5" );
    dataset->setDataset( "testdata" );

    // read the data from disk into the array
    dataset->initialize( xdm::primitiveType::kInt, fileshape,
      xdm::Dataset::kRead );
    dataset->deserialize( &result, xdm::DataSelectionMap() );
    dataset->finalize();

  } // end of reading

  // make sure the result data matches the input data
  BOOST_CHECK_EQUAL_COLLECTIONS( 
    result.begin(), result.end(), 
    data.begin(), data.end() );
}

BOOST_AUTO_TEST_CASE( compression ) {
  const size_t kLength = 1 << 20;
  xdm::VectorStructuredArray< int > data( kLength );
  std::fill( data.begin(), data.end(), 0 );

  // write the data uncompressed with chunking.
  {
    xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset(
      "Uncompressed.h5", xdmHdf::GroupPath(), "Data" ) );
    dataset->setUseChunkedIo( true );
    dataset->initialize(
      xdm::primitiveType::kInt,
      xdm::makeShape( kLength ),
      xdm::Dataset::kCreate );
    dataset->serialize( &data, xdm::DataSelectionMap() );
    dataset->finalize();
  }

  // write the same data compressed with chunking.
  {
    xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset(
      "Iscompressed.h5", xdmHdf::GroupPath(), "Data" ) );
    dataset->setUseChunkedIo( true );
    dataset->setUseCompression( true );
    dataset->initialize(
      xdm::primitiveType::kInt,
      xdm::makeShape( kLength ),
      xdm::Dataset::kCreate );
    dataset->serialize( &data, xdm::DataSelectionMap() );
    dataset->finalize();
  }

  size_t uncompressedSize = fileSize( "Uncompressed.h5" );
  size_t compressedSize = fileSize( "Iscompressed.h5" );
  // it's all zeroes, so compression should be at least factor of 2.
  BOOST_CHECK_LT( compressedSize, uncompressedSize / 2 );
}

} // namespace

