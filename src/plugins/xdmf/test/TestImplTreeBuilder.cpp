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
#define BOOST_TEST_MODULE ImplTreeBuilder
#include <boost/test/unit_test.hpp>

#include <xdmf/impl/TreeBuilder.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelectionMap.hpp>
#include <xdm/Dataset.hpp>
#include <xdm/TypedStructuredArray.hpp>
#include <xdm/VectorStructuredArray.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/StructuredTopology.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/UniformGrid.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace {

char const * const kTestDatasetFilename = "BuildTreeTest.h5";

// Create a simple test HDF5 file.
void createHdfFile() {
  xdm::RefPtr< xdmHdf::HdfDataset > dataset( new xdmHdf::HdfDataset );
  dataset->setFile( kTestDatasetFilename );
  xdmHdf::GroupPath path;
  path.push_back( "group1" );
  path.push_back( "group2" );
  dataset->setGroupPath( path );
  dataset->setDataset( "dataset" );
  xdm::RefPtr< xdm::VectorStructuredArray< double > > array(
    new xdm::VectorStructuredArray< double >( 9 ) );
  for ( int i = 1; i <= 9; i++ ) {
    (*array)[i-1] = i;
  }

  // write the array
  dataset->initialize(
    xdm::primitiveType::kDouble,
    xdm::makeShape( 3, 3),
    xdm::Dataset::kCreate );
  dataset->serialize( array.get(), xdm::DataSelectionMap() );
  dataset->finalize();
}

BOOST_AUTO_TEST_CASE( buildUniformDataItem ) {
  char const * const kXml =
  "<DataItem Name='test' "
  "  ItemType='Uniform'"
  "  Dimensions='3 3'"
  "  NumberType='Float'"
  "  Precision='8'"
  "  Format='HDF'>"
  "  BuildTreeTest.h5:/group1/group2/dataset"
  "</DataItem>";
  double resultData[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };

  createHdfFile();

  xmlDocPtr document = xmlParseDoc( reinterpret_cast< const xmlChar *>(kXml) );
  xmlNode * rootNode = xmlDocGetRootElement( document );

  xdmf::impl::TreeBuilder builder( document );
  xdm::RefPtr< xdm::UniformDataItem > item 
    = builder.buildUniformDataItem( rootNode );
  BOOST_REQUIRE( item );

  BOOST_CHECK_EQUAL( item->dataType(), xdm::primitiveType::kDouble );
  BOOST_CHECK_EQUAL( item->dataspace(), xdm::makeShape( 3, 3 ) );
  BOOST_REQUIRE( item->dataset() );
  BOOST_CHECK_EQUAL( item->dataset()->format(), "HDF" );

  xdm::RefPtr< xdmHdf::HdfDataset > dataset
    = xdm::dynamic_pointer_cast< xdmHdf::HdfDataset >( item->dataset() );
  BOOST_REQUIRE( dataset );

  BOOST_CHECK_EQUAL( dataset->file(), kTestDatasetFilename );
  xdmHdf::GroupPath path;
  path.push_back( "group1" );
  path.push_back( "group2" );
  BOOST_CHECK_EQUAL_COLLECTIONS(
    dataset->groupPath().begin(), dataset->groupPath().end(),
    path.begin(), path.end() );
  BOOST_CHECK_EQUAL( "dataset", dataset->dataset() );

  // check the data
  item->initializeDataset( xdm::Dataset::kRead );
  item->deserializeData();
  item->finalizeDataset();

  xdm::RefPtr< xdm::TypedStructuredArray< double > > array
    = item->typedArray< double >();
  BOOST_REQUIRE( array );
  BOOST_CHECK_EQUAL_COLLECTIONS(
    array->begin(), array->end(),
    resultData, resultData + 9 );

  xmlFreeDoc( document );
}

BOOST_AUTO_TEST_CASE( buildStructuredTopology ) {
  const char * kXml =
    "<Topology TopologyType='3DRectMesh' Dimensions='3 3 3'/>";

  xmlDocPtr document = xmlParseDoc( reinterpret_cast< const xmlChar * >(kXml) );
  xmlNode * rootNode = xmlDocGetRootElement( document );

  xdmf::impl::TreeBuilder builder( document );
  xdm::RefPtr< xdmGrid::Topology > result = builder.buildTopology( rootNode );

  BOOST_REQUIRE( result );

  xdm::RefPtr< xdmGrid::StructuredTopology > structured
    = xdm::dynamic_pointer_cast< xdmGrid::StructuredTopology >( result );
  BOOST_REQUIRE( structured );

  BOOST_CHECK_EQUAL( structured->shape(), xdm::makeShape( 3, 3, 3) );

  xmlFreeDoc( document );
}

BOOST_AUTO_TEST_CASE( buildStaticTree ) {
  xmlDocPtr document = xmlParseFile( "test_document1.xmf" );
  BOOST_REQUIRE( document );

  xdmf::impl::TreeBuilder builder( document );
  xdm::RefPtr< xdm::Item > result = builder.buildTree();
  BOOST_REQUIRE( result );

  xdm::RefPtr< xdmGrid::UniformGrid > grid
    = xdm::dynamic_pointer_cast< xdmGrid::UniformGrid >( result );
  BOOST_REQUIRE( grid );

  // Check the time.
  xdm::RefPtr< xdmGrid::Time > time = grid->time();
  BOOST_REQUIRE( time );
  BOOST_CHECK_EQUAL( time->value(), 0.0 );

  // Check the topology.
  xdm::RefPtr< xdmGrid::Topology > topology = grid->topology();
  BOOST_REQUIRE( topology );
  xdm::RefPtr< xdmGrid::StructuredTopology > structured =
    xdm::dynamic_pointer_cast< xdmGrid::StructuredTopology >( topology );
  BOOST_REQUIRE( structured );
  BOOST_CHECK_EQUAL( structured->shape(), xdm::makeShape( 383, 129, 129 ) );

  // Check the geometry.
  xdm::RefPtr< xdmGrid::Geometry > geometry = grid->geometry();
  BOOST_REQUIRE( geometry );
  xdm::RefPtr< xdmGrid::TensorProductGeometry > tpGeo =
    xdm::dynamic_pointer_cast< xdmGrid::TensorProductGeometry >( geometry );
  BOOST_REQUIRE( tpGeo );
  BOOST_CHECK_EQUAL( tpGeo->dimension(), 3 );

  // Check the attributes.
  BOOST_REQUIRE_EQUAL( grid->numberOfChildren(), 3 );
  char * names[] = { "E", "B", "InternalCell" };
  xdmGrid::Attribute::Type types[] = {
    xdmGrid::Attribute::kVector,
    xdmGrid::Attribute::kVector,
    xdmGrid::Attribute::kScalar
  };
  xdmGrid::Attribute::Center centers[] = {
    xdmGrid::Attribute::kCell,
    xdmGrid::Attribute::kCell,
    xdmGrid::Attribute::kCell,
  };
  for ( int i = 0; i < 3; i++ ) {
    xdm::RefPtr< xdmGrid::Attribute > attr = grid->child( i );
    BOOST_CHECK_EQUAL( attr->name(), names[i] );
    BOOST_CHECK_EQUAL( attr->dataType(), types[i] );
    BOOST_CHECK_EQUAL( attr->centering(), centers[i] );
  }
}

} // namespace

