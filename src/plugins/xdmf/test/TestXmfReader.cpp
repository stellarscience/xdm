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
#define BOOST_TEST_MODULE XmfReader 
#include <boost/test/unit_test.hpp>

#include <xdmf/XmfReader.hpp>
#include <xdmf/XmfWriter.hpp>

#include <xdm/ArrayAdapter.hpp>
#include <xdm/Item.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/UpdateVisitor.hpp>
#include <xdm/VectorStructuredArray.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/RectilinearMesh.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/Time.hpp>
#include <xdmGrid/UniformGrid.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <algorithm>
#include <fstream>

#include <cmath>

double function( double x, double y ) {
  double xrad = x * ( 6.28 / 360.0 );
  double yrad = y * ( 6.28 / 360.0 );
  return std::sin( xrad + yrad );
}

double timeFunction( size_t step ) {
  return step;
}

static const int kMeshSize[2] = { 10, 5 };
static const double kRange[2][2] = { {0.0, 360.0}, {-90.0, 90.0} };

xdm::RefPtr< xdmGrid::UniformGrid > build2DGrid() {
  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid );

  // Time
  {
    grid->setTime( xdm::makeRefPtr( new xdmGrid::Time( 0.0 ) ) );
  }

  // Topology
  {
     xdm::RefPtr< xdmGrid::RectilinearMesh > topology( new xdmGrid::RectilinearMesh );
    topology->setShape( xdm::makeShape( kMeshSize[0]-1, kMeshSize[1]-1 ) );
    grid->setTopology( topology );
  }

  // Geometry
  xdm::RefPtr< xdm::VectorStructuredArray< double > > meshValues[2];
  {
    xdm::RefPtr< xdmGrid::TensorProductGeometry > geometry(
      new xdmGrid::TensorProductGeometry( 2 ) );
    for ( int i = 0; i < 2; i++ ) {
      xdm::RefPtr< xdm::UniformDataItem > dataItem( new xdm::UniformDataItem );
      dataItem->setDataType( xdm::primitiveType::kDouble );
      dataItem->setDataspace( xdm::makeShape( kMeshSize[i] ) );
      meshValues[i] = xdm::makeRefPtr(
        new xdm::VectorStructuredArray< double >( kMeshSize[i] ) );
      for ( int j = 0; j < kMeshSize[i]; j++ ) {
        (*meshValues[i])[j] = kRange[i][0] + j * ( (kRange[i][1] - kRange[i][0]) / kMeshSize[i] );
      }
      dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( meshValues[i] ) ) );
      geometry->setCoordinateValues( i, dataItem );
    }
    grid->setGeometry( geometry );
  }

  // Give it an attribute.
  {
    xdm::RefPtr< xdmGrid::Attribute > attribute( new xdmGrid::Attribute(
      xdmGrid::Attribute::kScalar,
      xdmGrid::Attribute::kNode ) );
    attribute->setName( "attr" );
    xdm::RefPtr< xdm::VectorStructuredArray< double > > attrValues(
      new xdm::VectorStructuredArray< double >( kMeshSize[1] * kMeshSize[0] ) );
    for ( int j = 0; j < kMeshSize[1]; j++ ) {
      for ( int i = 0; i < kMeshSize[0]; i++ ) {
        double xpoint = (*meshValues[0])[i];
        double ypoint = (*meshValues[1])[j];
        (*attrValues)[j*kMeshSize[0] + i] = function( xpoint, ypoint );
      }
    }
    xdm::RefPtr< xdm::UniformDataItem > dataItem(
      new xdm::UniformDataItem(
        xdm::primitiveType::kDouble,
        xdm::makeShape( kMeshSize[1], kMeshSize[0] ) ) );
    dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( attrValues, true ) ) );
    attribute->setDataItem( dataItem );
    grid->addAttribute( attribute );
  }

  return grid;
}

// Function to build a 2D grid and write it to disk.
void write2DGrid( const xdm::FileSystemPath& path ) {
  xdmf::XmfWriter writer;
  writer.open( path, xdm::Dataset::kCreate );
  writer.write( build2DGrid(), 0 );
  writer.close();
}

// Write time dependent data.
void writeTimeGrid( const xdm::FileSystemPath& path ) {
  xdm::RefPtr< xdmGrid::UniformGrid > grid = build2DGrid();

  xdmf::XmfWriter writer;
  writer.open( path, xdm::Dataset::kCreate );
  xdm::RefPtr< xdmGrid::Attribute > attr = grid->attributeByName( "attr" );
  xdm::RefPtr< xdm::UniformDataItem > data = attr->dataItem();
  xdm::RefPtr< xdm::TypedStructuredArray< double > > array = data->typedArray<double>();
  for ( int step = 0; step < 5; ++step ) {
    grid->time()->setValue( static_cast< double >( step ) );
    std::fill( array->begin(), array->end(), timeFunction( step ) );
    writer.write( grid, step );
  }
  writer.close();
}

BOOST_AUTO_TEST_CASE( parseFile ) {
  xdm::FileSystemPath testFilePath( "test_document1.xmf" );

  xdmf::XmfReader reader;
  xdmFormat::ReadResult result = reader.readItem( testFilePath );
  BOOST_CHECK( result.item() );
}

BOOST_AUTO_TEST_CASE( invalidDocument ) {
  const char * kXml = "<Xdmf Version='2.1'><foo/></Xdmf>";
  const char * kTestFileName = "invalidDocumentFile.xmf";

  {
    std::ofstream testfile( kTestFileName );
    testfile << kXml;
  }

  xdmf::XmfReader reader;
  BOOST_CHECK_THROW( reader.readItem( xdm::FileSystemPath( kTestFileName ) ),
    xdmFormat::FileReadError );
}

BOOST_AUTO_TEST_CASE( grid2DRoundtrip ) {
  const xdm::FileSystemPath testFilePath( "grid2DRoundtrip.xmf" );
  const xdm::FileSystemPath testHdfFilePath( "grid2DRoundtrip.xmf.h5" );

  xdm::remove( testFilePath );
  xdm::remove( testHdfFilePath );

  write2DGrid( testFilePath );

  xdmf::XmfReader reader;
  xdmFormat::ReadResult result = reader.readItem( testFilePath );
  BOOST_CHECK_EQUAL( result.seriesSteps(), 1 ); // one timestep in that file
  xdm::RefPtr< xdm::Item > item = result.item();
  BOOST_REQUIRE( item );

  xdm::RefPtr< xdmGrid::UniformGrid > grid =
    xdm::dynamic_pointer_cast< xdmGrid::UniformGrid >( item );
  BOOST_REQUIRE( grid );

  xdm::RefPtr< xdmGrid::StructuredTopology > topology =
    xdm::dynamic_pointer_cast< xdmGrid::StructuredTopology >( grid->topology() );
  BOOST_REQUIRE( topology );
  BOOST_CHECK_EQUAL( topology->shape(), xdm::makeShape( kMeshSize[0]-1, kMeshSize[1]-1 ) );

  BOOST_CHECK_EQUAL( grid->geometry()->dimension(), 2 );
  BOOST_CHECK_EQUAL( grid->geometry()->numberOfNodes(), kMeshSize[0] * kMeshSize[1] );

  xdm::RefPtr< xdmGrid::Attribute > attribute = grid->attributeByName( "attr" );
  BOOST_REQUIRE( attribute );
  xdm::RefPtr< xdm::UniformDataItem > attributeData = attribute->dataItem();
  BOOST_REQUIRE( attributeData );

  double nodePositions[kMeshSize[0]][kMeshSize[1]][2];
  for ( int i = 0; i < kMeshSize[0]; ++i ) {
    for ( int j = 0; j < kMeshSize[1]; ++j ) {
      nodePositions[i][j][0] = kRange[0][0] + i * (kRange[0][1] - kRange[0][0]) / kMeshSize[0];
      nodePositions[i][j][1] = kRange[1][0] + j * (kRange[1][1] - kRange[1][0]) / kMeshSize[1];
    }
  }

  // Check contiguous indexing. The loops below are in j-i order because the
  // XDM convention is X varies fastest when indexing contiguously through the
  // nodes.
  int index = 0;
  for ( int j = 0; j < kMeshSize[1]; ++j ) {
    for ( int i = 0; i < kMeshSize[0]; ++i ) {
      xdmGrid::Node node = grid->node( index );
      BOOST_CHECK_EQUAL( nodePositions[i][j][0], node[0] );
      BOOST_CHECK_EQUAL( nodePositions[i][j][1], node[1] );
      BOOST_CHECK_EQUAL(
        attributeData->atIndex< double >( index++ ),
        function( node[0], node[1] ) );
    }
  }
}

BOOST_AUTO_TEST_CASE( temporalCollectionRoundtrip ) {
  const xdm::FileSystemPath testFilePath( "temporalCollectionRoundtrip.xmf" );
  const xdm::FileSystemPath hdfFilePath( "temporalCollectionRoundtrip.xmf.h5" );

  xdm::remove( testFilePath );
  xdm::remove( hdfFilePath );

  writeTimeGrid( testFilePath );

  xdmFormat::ReadResult result;
  {
    // make sure data is valid outside the scope of the reader.
    xdmf::XmfReader reader;
    result = reader.readItem( testFilePath );
  }
  BOOST_CHECK_EQUAL( result.seriesSteps(), 5 );
  BOOST_REQUIRE( result.item() );

  xdm::RefPtr< xdmGrid::UniformGrid > g =
    xdm::dynamic_pointer_cast< xdmGrid::UniformGrid >( result.item() );
  BOOST_REQUIRE( g );

  xdm::RefPtr< xdm::UniformDataItem > data = g->attributeByName( "attr" )->dataItem();
  // The bounds below are Y-X order.
  BOOST_CHECK_EQUAL( data->dataspace(),
    xdm::makeShape( kMeshSize[1], kMeshSize[0] ) );
  for ( size_t step = 0; step < 5; ++step ) {
    xdm::updateToIndex( *g, step );
    BOOST_CHECK_EQUAL( g->time()->value(), step );
    double value = data->atLocation< double >( 2, 5 );
    BOOST_CHECK_EQUAL( value, step );
  }

  // Go back in time.
  xdm::updateToIndex( *g, 2 );
  BOOST_CHECK_EQUAL( g->time()->value(), 2.0 );
  BOOST_CHECK_EQUAL( data->atLocation< double >( 2, 5 ), 2.0 );
}
