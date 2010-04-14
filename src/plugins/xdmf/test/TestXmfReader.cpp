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
#include <xdm/VectorStructuredArray.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/RectilinearMesh.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/UniformGrid.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <fstream>

#include <cmath>

double function( double x, double y ) {
  double xrad = x * ( 6.28 / 360.0 );
  double yrad = y * ( 6.28 / 360.0 );
  return std::sin( xrad + yrad );
}

static const int kMeshSize[2] = { 10, 5 };
static const double kRange[2][2] = { {0.0, 360.0}, {-90.0, 90.0} };

xdm::RefPtr< xdmGrid::UniformGrid > build2DGrid() {
  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid );

  // Topology
  {
    // FIXME: Dimensions below are y-x order.
    xdm::RefPtr< xdmGrid::RectilinearMesh > topology( new xdmGrid::RectilinearMesh );
    topology->setShape( xdm::makeShape( kMeshSize[1], kMeshSize[0] ) );
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
    dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( attrValues ) ) );
    attribute->setDataItem( dataItem );
    grid->addAttribute( attribute );
  }

  return grid;
}

// Function to build a 2D grid and write it to disk.
void write2DGrid( const xdm::FileSystemPath& path ) {
  xdmf::XmfWriter writer;
  writer.writeItem( build2DGrid(), path );
}

BOOST_AUTO_TEST_CASE( parseFile ) {
  xdm::FileSystemPath testFilePath( "test_document1.xmf" );

  xdmf::XmfReader reader;
  xdm::RefPtr< xdm::Item > result = reader.readItem( testFilePath );
  BOOST_CHECK( result );
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
    xdmf::ValidationError );
}

BOOST_AUTO_TEST_CASE( grid2DRoundtrip ) {
  const xdm::FileSystemPath testFilePath( "grid2DRoundtrip.xmf" );
  const xdm::FileSystemPath testHdfFilePath( "grid2DRoundtrip.xmf.h5" );

  xdm::remove( testFilePath );
  xdm::remove( testHdfFilePath );

  write2DGrid( testFilePath );

  xdmf::XmfReader reader;
  xdm::RefPtr< xdm::Item > result = reader.readItem( testFilePath );
  BOOST_REQUIRE( result );

  xdm::RefPtr< xdmGrid::UniformGrid > grid =
    xdm::dynamic_pointer_cast< xdmGrid::UniformGrid >( result );
  BOOST_REQUIRE( grid );

  xdm::RefPtr< xdmGrid::StructuredTopology > topology =
    xdm::dynamic_pointer_cast< xdmGrid::StructuredTopology >( grid->topology() );
  BOOST_REQUIRE( topology );
  BOOST_CHECK_EQUAL( topology->shape(), xdm::makeShape( kMeshSize[0], kMeshSize[1] ) );

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
