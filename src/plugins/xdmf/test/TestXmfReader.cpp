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

// Function to build a 2D grid and write it to disk.
void write2DGrid( const xdm::FileSystemPath& path ) {
  static const int kMeshSize = 10;
  static const double xrange[] = {0.0, 360.0};
  static const double yrange[] = {-90.0, 90.0};
  static const double * ranges[] = {xrange, yrange};

  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid );

  // Topology
  {
    xdm::RefPtr< xdmGrid::RectilinearMesh > topology( new xdmGrid::RectilinearMesh );
    topology->setShape( xdm::makeShape( kMeshSize, kMeshSize ) );
    grid->setTopology( topology );
  }

  // Geometry
  xdm::RefPtr< xdm::VectorStructuredArray< double > > meshValues[2];
  {
    xdm::RefPtr< xdmGrid::TensorProductGeometry > geometry(
      new xdmGrid::TensorProductGeometry( 2 ) );
    for ( int i = 0; i < 2; i++ ) {
      xdm::RefPtr< xdm::UniformDataItem > dataItem( new xdm::UniformDataItem );
      dataItem->setDataspace( xdm::makeShape( kMeshSize ) );
      meshValues[i] = xdm::makeRefPtr(
        new xdm::VectorStructuredArray< double >( kMeshSize ) );
      for ( int j = 0; j < kMeshSize; j++ ) {
        (*meshValues[i])[j] = ranges[i][0] + j * ( (ranges[i][1] - ranges[i][0]) / kMeshSize );
      }
      dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( meshValues[i] ) ) );
      xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset );
      dataset->setFile( path.pathString() + ".h5" );
      dataset->setDataset( (i == 0) ? "xgrid" : "ygrid" );
      dataItem->setDataset( dataset );
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
      new xdm::VectorStructuredArray< double >( kMeshSize * kMeshSize ) );
    for ( int i = 0; i < kMeshSize; i++ ) {
      for ( int j = 0; j < kMeshSize; j++ ) {
        double xpoint = (*meshValues[0])[i];
        double ypoint = (*meshValues[1])[j];
        (*attrValues)[i*kMeshSize + j] = xpoint*xpoint + ypoint*ypoint;
      }
    }
    xdm::RefPtr< xdm::UniformDataItem > dataItem(
      new xdm::UniformDataItem(
        xdm::primitiveType::kDouble,
        xdm::makeShape( kMeshSize, kMeshSize ) ) );
    dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( attrValues ) ) );
    xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset );
    dataset->setFile( path.pathString() + ".h5" );
    dataset->setDataset( "attr" );
    dataItem->setDataset( dataset );
    attribute->setDataItem( dataItem );
    grid->addAttribute( attribute );
  }

  xdmf::XmfWriter writer;
  writer.writeItem( grid, xdm::FileSystemPath( path.pathString() ) );
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

  write2DGrid( testFilePath );

  xdmf::XmfReader reader;
  xdm::RefPtr< xdm::Item > result = reader.readItem( testFilePath );
  BOOST_REQUIRE( result );

  xdm::RefPtr< xdmGrid::UniformGrid > grid =
    xdm::dynamic_pointer_cast< xdmGrid::UniformGrid >( result );
  BOOST_REQUIRE( grid );
}
