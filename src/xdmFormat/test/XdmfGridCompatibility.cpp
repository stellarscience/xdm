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
#include <xdm/XmlOutputStream.hpp>

#include <xdmFormat/XdmfHelpers.hpp>

#include <xdmGrid/CollectionGrid.hpp>
#include <xdmGrid/Domain.hpp>
#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/RectilinearMesh.hpp>
#include <xdmGrid/Time.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <cmath>

std::vector< double > createGridPoints( int size ) {
  std::vector< double > result( size );
  for ( int i = 0; i < size; ++i ) {
    result[i] = i * ( 6.28 / size );
  }
  return result;
}

std::vector< float > createAttributeData( 
  int sizex, 
  int sizey, 
  int sizez, 
  float t = 0.0f) {
  std::vector< float > result( sizex * sizey * sizez );
  for ( int i = 0; i < sizex; ++i ) {
    for ( int j = 0; j < sizey; ++j ) {
      for ( int k = 0; k < sizez; ++k ) {
        int location = k * sizex * sizey + j * sizey + i;
        float x = i * ( 6.28 / sizex );
        float y = j * ( 6.28 / sizey );
        float z = k * ( 6.28 / sizez );
        result[location] = std::sin( x + 2 * y + 4 * z + t );
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

  // add a node centered scalar attribute
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
  xdm::RefPtr< xdm::XmlObject > xdmf = xdmFormat::createXdmfRoot();
  xdmf->appendChild( collector.result() );

  std::ofstream xdmfFile("XdmfGridCompatibility.staticGrid.xmf");
  xdmfFile << *xdmf;
  xdmfFile.close();
}

TEST( XdmfGridCompatibility, timeSeries ) {
  // open an XML stream for incremental output.
  std::ofstream xdmfFile("XdmfGridCompatibility.timeSeries.xmf");
  xdm::XmlOutputStream xmlStream( xdmfFile );
  
  {
    // domain for the simulation
    xdm::RefPtr< xdmGrid::Domain > domain( new xdmGrid::Domain );

    // the temporal collection grid for the full simulation.
    xdm::RefPtr< xdmGrid::CollectionGrid > temporalGrid( 
      new xdmGrid::CollectionGrid( xdmGrid::CollectionGrid::kTemporal ) );
    domain->addGrid( temporalGrid );

    // write the beginning of the grid specification to the stream.
    xdm::CollectMetadataOperation collector;
    domain->accept( collector );
    xdm::RefPtr< xdm::XmlObject > xdmf = xdmFormat::createXdmfRoot();
    xdmf->appendChild( collector.result() );
    xmlStream.openContext( xdmf );
  } // leave scope, freeing all objects allocated above

  // since the geometry and topology of the grid will remain constant throughout
  // the simulation, we make a single object here to share.
  xdm::RefPtr< xdmGrid::TensorProductGeometry > sharedGeometry( 
    new xdmGrid::TensorProductGeometry( 3 ) );
  std::vector< double > vertexData = createGridPoints( 10 );
  xdm::RefPtr< xdm::StructuredArray > geometryArray =
    xdm::createStructuredArray( &vertexData[0], xdm::makeShape( 10 ) );
  xdm::RefPtr< xdmHdf::HdfDataset > geometryDataset( new xdmHdf::HdfDataset );
  geometryDataset->setFile( "XdmfGridCompatibility.timeSeries.h5" );
  geometryDataset->setDataset( "gridValues" );
  xdm::RefPtr< xdm::UniformDataItem > geodata( 
    new xdm::UniformDataItem( xdm::makeShape( 10 ) ) );
  geodata->setArray( geometryArray );
  geodata->setDataset( geometryDataset );
  for ( int i = 0; i < 3; ++i ) {
    sharedGeometry->setCoordinateValues( i, geodata.get() );
  }

  xdm::RefPtr< xdmGrid::RectilinearMesh > sharedTopology( 
    new xdmGrid::RectilinearMesh );
  sharedTopology->setShape( xdm::makeShape( 10, 10, 10 ) );

  double timeval = 0.0;
  for ( unsigned int step = 0; step <= 3; ++step ) {
    timeval += 0.1;

    // create a grid and set it up to use the shared geometry and topology
    xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid );
    grid->setTopology( sharedTopology.get() );
    grid->setGeometry( sharedGeometry.get() );

    // construct the time element for this time step.
    xdm::RefPtr< xdmGrid::Time > time( new xdmGrid::Time );
    time->setValue( timeval );
    grid->setTime( time );

    // construct a cell centered scalar attribute for this time step.
    xdm::RefPtr< xdmGrid::Attribute > attribute( 
      new xdmGrid::Attribute( 
        xdmGrid::Attribute::kScalar,
        xdmGrid::Attribute::kCell ) );
    attribute->setName( "FunctionValues" );
    grid->appendChild( attribute );

    // build the data for the attribute.
    std::vector< float > attrvalues = createAttributeData( 9, 9, 9, timeval );
    xdm::RefPtr< xdm::StructuredArray > attrArray = 
      xdm::createStructuredArray( 
        &attrvalues[0], 
        xdm::makeShape( 9*9*9 ) );
    xdm::RefPtr< xdmHdf::HdfDataset > attrDataset( new xdmHdf::HdfDataset );
    attrDataset->setFile( "XdmfGridCompatibility.timeSeries.h5" );
    attrDataset->setGroup( "FunctionValues" );
    std::stringstream stepStr;
    stepStr << std::setfill('0') << std::setw(5) << step;
    attrDataset->setDataset( stepStr.str() );
    xdm::RefPtr< xdm::UniformDataItem > attrData( 
      new xdm::UniformDataItem( xdm::makeShape( 9, 9, 9 ) ) );
    attrData->setArray( attrArray.get() );
    attrData->setDataset( attrDataset.get() );
    attribute->appendChild( attrData );

    // the grid definition is complete, write to the XML output stream
    xdm::CollectMetadataOperation collector;
    grid->accept( collector );
    xmlStream.writeObject( collector.result() );

    // finally, serialize all datasets that need it
    xdm::SerializeDataOperation serializer;
    grid->accept( serializer );
  }
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

