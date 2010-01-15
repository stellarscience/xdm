//-----------------------------------------------------------------------------
// These are tests to verify format compatibility with ParaView's XDMF Library
//-----------------------------------------------------------------------------
#include <gtest/gtest.h>

#include <xdm/CollectMetadataOperation.hpp>
#include <xdm/SerializeDataOperation.hpp>
#include <xdm/StructuredArray.hpp>
#include <xdm/TemplateStructuredArray.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/XmlObject.hpp>

#include <xdmGrid/Domain.hpp>
#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/RectilinearMesh.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>

#include <cmath>

std::vector< double > createGridPoints( int size ) {
  std::vector< double > result( size );
  for ( int i = 0; i < size; ++i ) {
    result[i] = i * ( 6.28 / size );
  }
  return result;
}

std::vector< float > createAttributeData( int sizex, int sizey, int sizez ) {
  std::vector< float > result( sizex * sizey * sizez );
  for ( int i = 0; i < sizex; ++i ) {
    for ( int j = 0; j < sizey; ++j ) {
      for ( int k = 0; k < sizez; ++k ) {
        int location = k * sizex * sizey + j * sizey + i;
        float x = i * ( 6.28 / sizex );
        float y = j * ( 6.28 / sizey );
        float z = k * ( 6.28 / sizez );
        result[location] = std::sin( x + 2 * y + 4 * z );
      }
    }
  }
  return result;
}

TEST( XdmfGridCompatibility, staticGrid ) {
  xdm::RefPtr< xdmGrid::Domain > domain( new xdmGrid::Domain );

  // build the grid for the domain
  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid );
  domain->addGrid( grid );

  // build the topology for the grid.
  xdm::RefPtr< xdmGrid::RectilinearMesh > topology( 
    new xdmGrid::RectilinearMesh );
  topology->setShape( xdm::makeShape("10 10 10") );
  grid->setTopology( topology );

  // build the geometry for the grid.
  xdm::RefPtr< xdmGrid::TensorProductGeometry > geometry( 
    new xdmGrid::TensorProductGeometry( 3 ) );
  grid->setGeometry( geometry );
  
  // data in memory
  std::vector< double > vertices = createGridPoints( 10 );
  xdm::DataShape<> arrayShape(1);
  arrayShape[0] = 10;
  xdm::RefPtr< xdm::StructuredArray > array 
    = xdm::createStructuredArray( &vertices[0], arrayShape );

  // dataset on disk
  xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset );
  dataset->setFile( "XdmfGridCompatibility.h5" );
  dataset->setDataset( "staticGrid" );
  xdm::DataShape<> fileShape(1);
  fileShape[0] = 10;

  // the data item to map the array in memory to the file
  // since the coordinates are the same in x, y, and z we will share the
  // dataitem.
  xdm::RefPtr< xdm::UniformDataItem > sharedData( 
    new xdm::UniformDataItem( fileShape ) );
  sharedData->setArray( array );
  sharedData->setDataset( dataset );
  for ( int i = 0; i < 3; ++i ) {
    geometry->setCoordinateValues( i, sharedData.get() );
  }

  // add a cell centered vector attribute
  xdm::RefPtr< xdmGrid::Attribute > attribute( 
    new xdmGrid::Attribute( 
      xdmGrid::Attribute::kScalar, 
      xdmGrid::Attribute::kNode ) );
  grid->appendChild( attribute );
  
  // create the data for the attribute.
  std::vector< float > data = createAttributeData( 10, 10, 10 );
  xdm::RefPtr< xdm::UniformDataItem > attributeDataItem(
    new xdm::UniformDataItem( xdm::makeShape( "10 10 10" ) ) );
  xdm::RefPtr< xdmHdf::HdfDataset > attrds( new xdmHdf::HdfDataset );
  attrds->setFile( "attribute.h5" );
  attrds->setDataset( "values" );
  xdm::RefPtr< xdm::StructuredArray > attrvalues 
    = xdm::createStructuredArray( &data[0], xdm::makeShape( "1000" ) );
  attributeDataItem->setArray( attrvalues );
  attributeDataItem->setDataset( attrds );
  attribute->appendChild( attributeDataItem );

  // write the tree's data to disk
  xdm::SerializeDataOperation serializer;
  domain->accept( serializer );

  // collect the tree's metadata
  xdm::CollectMetadataOperation collector;
  domain->accept( collector );
  xdm::RefPtr< xdm::XmlObject > xml( new xdm::XmlObject( "Xdmf" ) );
  xml->appendAttribute( "Version", "2.1" );
  xml->appendChild( collector.result() );

  std::ofstream xdmfFile("XdmfGridCompatibility.staticGrid.xmf");
  xdmfFile << *xml;
  xdmfFile.close();
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

