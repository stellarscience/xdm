//-----------------------------------------------------------------------------
// These are tests to verify format compatibility with ParaView's XDMF Library
//-----------------------------------------------------------------------------
#include <gtest/gtest.h>

#include <xdm/CollectMetadataOperation.hpp>
#include <xdm/HdfDataset.hpp>
#include <xdm/StructuredArray.hpp>
#include <xdm/TemplateStructuredArray.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/XmlObject.hpp>

#include <xdmGrid/Domain.hpp>
#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/RectilinearMesh.hpp>

#include <algorithm>
#include <sstream>

TEST( XdmfGridCompatibility, staticGrid ) {
  xdm::RefPtr< xdmGrid::Domain > domain( new xdmGrid::Domain );

  // build the grid for the domain
  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid );
  domain->addGrid( grid );

  // build the topology for the grid.
  xdm::RefPtr< xdmGrid::RectilinearMesh > topology( 
    new xdmGrid::RectilinearMesh );
  topology->setShape( xdm::makeShape("3 3 3") );
  grid->setTopology( topology );

  // build the geometry for the grid.
  xdm::RefPtr< xdmGrid::TensorProductGeometry > geometry( 
    new xdmGrid::TensorProductGeometry( 3 ) );
  grid->setGeometry( geometry );
  
  // data in memory
  double vertices[3];
  vertices[0] = 0.0;
  vertices[1] = 1.0;
  vertices[2] = 2.0;
  xdm::DataShape<> arrayShape(1);
  arrayShape[0] = 3;
  xdm::RefPtr< xdm::StructuredArray > array 
    = xdm::createStructuredArray( vertices, arrayShape );

  // dataset on disk
  xdm::RefPtr< xdm::HdfDataset > dataset( new xdm::HdfDataset );
  dataset->setFile( "XdmfGridCompatibility.h5" );
  dataset->setDataset( "staticGrid" );
  xdm::DataShape<> fileShape(1);
  fileShape[0] = 3;

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

  // write the tree's metadata
  xdm::CollectMetadataOperation collector;
  domain->accept( collector );
  xdm::RefPtr< xdm::XmlObject > xml = collector.result();
  std::stringstream result;
  result << *xml;

  ASSERT_EQ( "", result.str() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

