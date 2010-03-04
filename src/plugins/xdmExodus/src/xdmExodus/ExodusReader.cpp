//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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
#include <xdmExodus/ExodusReader.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Domain.hpp>
#include <xdmGrid/MultiArrayGeometry.hpp>
#include <xdmGrid/Polyvertex.hpp>
#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>
#include <xdmGrid/UnstructuredTopologyConventions.hpp>

#include <xdm/ArrayAdapter.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/VectorStructuredArray.hpp>

#include <exodusII.h>

#include <algorithm>
#include <functional>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#define EXODUS_CHECK( functionCall, errorString ) \
  if ( (#functionCall) < 0 ) { \
    throw std::runtime_error( (#errorString) ); \
  }

XDM_EXODUS_NAMESPACE_BEGIN

namespace {

// String type for Exodus
struct ExodusString {
  char raw[ MAX_STR_LENGTH + 1 ];
  char* ptr() { return raw; }
  std::string string() { return std::string( raw ); }
};

// Helpers that create a UniformDataItem from a StructuredArray. This is done frequently.
// First version takes one dimension.
xdm::RefPtr< xdm::UniformDataItem > makeDataItem(
  xdm::RefPtr< xdm::StructuredArray > vector,
  xdm::primitiveType::Value primType,
  std::size_t firstExtent ) {

  xdm::RefPtr< xdm::UniformDataItem > dataItem(
    new xdm::UniformDataItem( primType, xdm::makeShape( firstExtent ) ) );
  dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( vector ) ) );
}

// Second version takes two dimensions.
xdm::RefPtr< xdm::UniformDataItem > makeDataItem(
  xdm::RefPtr< xdm::StructuredArray > vector,
  xdm::primitiveType::Value primType,
  std::size_t firstExtent,
  std::size_t secondExtent ) {

  xdm::RefPtr< xdm::UniformDataItem > dataItem(
    new xdm::UniformDataItem( primType, xdm::makeShape( firstExtent, secondExtent ) ) );
  dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( vector ) ) );
}

xdmGrid::CellType::Type lookupCellType( std::size_t nodesPerCell, const std::string& cellName ) {
  // Make a map that takes the number of nodes and the Exodus name and gives a CellType.
  typedef std::map< std::string, xdmGrid::CellType::Type > NameMap;
  typedef std::map< std::size_t, NameMap > NumberOfNodesMap;
  static NumberOfNodesMap nodeMap;
  if ( nodeMap.empty() ) {
    nodeMap[ 1 ][ "CIRCLE" ] = xdmGrid::CellType::Circle;
    nodeMap[ 1 ][ "SPHERE" ] = xdmGrid::CellType::Sphere;
    nodeMap[ 2 ][ "TRUSS" ] = xdmGrid::CellType::Beam;
    nodeMap[ 2 ][ "BEAM" ] = xdmGrid::CellType::Beam;
    nodeMap[ 2 ][ "SHELL(2D)" ] = xdmGrid::CellType::Beam;
    nodeMap[ 3 ][ "TRIANGLE" ] = xdmGrid::CellType::Triangle;
    nodeMap[ 3 ][ "TRUSS" ] = xdmGrid::CellType::QuadraticBeam;
    nodeMap[ 3 ][ "BEAM" ] = xdmGrid::CellType::QuadraticBeam;
    nodeMap[ 3 ][ "SHELL(2D)" ] = xdmGrid::CellType::QuadraticBeam;
    nodeMap[ 4 ][ "QUAD" ] = xdmGrid::CellType::Quad;
    nodeMap[ 4 ][ "SHELL(3D)" ] = xdmGrid::CellType::Quad;
    nodeMap[ 4 ][ "TETRA" ] = xdmGrid::CellType::Tetra;
    nodeMap[ 5 ][ "PYRAMID" ] = xdmGrid::CellType::Pyramid;
    nodeMap[ 6 ][ "WEDGE" ] = xdmGrid::CellType::Wedge;
    nodeMap[ 7 ][ "TRIANGLE" ] = xdmGrid::CellType::QuadraticTriangle;
    nodeMap[ 8 ][ "HEX" ] = xdmGrid::CellType::Hex;
    nodeMap[ 9 ][ "QUAD" ] = xdmGrid::CellType::QuadraticQuad;
    nodeMap[ 9 ][ "SHELL(3D)" ] = xdmGrid::CellType::QuadraticQuad;
    nodeMap[ 11 ][ "TETRA" ] = xdmGrid::CellType::QuadraticTetra;
    nodeMap[ 13 ][ "PYRAMID" ] = xdmGrid::CellType::QuadraticPyramid;
    nodeMap[ 16 ][ "WEDGE" ] = xdmGrid::CellType::QuadraticWedge;
    nodeMap[ 21 ][ "HEX" ] = xdmGrid::CellType::QuadraticHex;
  }

  // Find the correct CellType by first matching number of nodes and then matching the
  // Exodus name.
  NumberOfNodesMap::const_iterator matchingNodes = nodeMap.find( nodesPerCell );
  if ( matchingNodes != nodeMap.end() ) {
    const NameMap& matchingNames = matchingNodes->second;
    NameMap::const_iterator matchingName = matchingNames.find( cellName );
    if ( matchingName != matchingNames.end() ) {
      return matchingName->second;
    }
  }

  std::stringstream ss;
  ss << "A cell type was not found for Exodus name " << cellName << " with number of"
    " nodes per cell.";
  throw std::runtime_error( ss.str() );
}

} // anon namespace

ExodusReader::ExodusReader() {
}

ExodusReader::~ExodusReader() {
}

xdm::RefPtr< xdm::Item > ExodusReader::readItem( const std::string& filename ) {
  // Open the file and get some info.
  float version;
  int doubleWordSize = sizeof( double );
  int storedDataWordSize = 0;
  int fileId = ex_open(
    filename.c_str(),
    EX_READ,
    &doubleWordSize,
    &storedDataWordSize,
    &version );
  if ( fileId < 0 ) {
    throw std::runtime_error( "Could not open ExodusII filename: " + filename );
  }

  ex_init_params gridParameters;
  EXODUS_CHECK(  ex_get_init_ext( fileId, &gridParameters ), "ex_get_init_ext returned an error." );

  //---------------NODES-------------------
  // Read the nodes. For Exodus, there is only ever one set of nodes that all element blocks
  // refer to.
  std::vector< xdm::RefPtr< xdm::VectorStructuredArray< double > > > xyzCoords( 3 );
  xdm::RefPtr< xdmGrid::MultiArrayGeometry > geom(
    new xdmGrid::MultiArrayGeometry( gridParameters.num_dim ) );
  for( std::size_t dim = 0; dim < gridParameters.num_dim; ++dim ) {
    xyzCoords[ dim ] = new xdm::VectorStructuredArray< double >( gridParameters.num_nodes );
    xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
      xyzCoords[ dim ], xdm::primitiveType::kDouble, gridParameters.num_nodes );
    geom->setCoordinateValues( dim, dataItem );
  }
  EXODUS_CHECK(
    ex_get_coord( fileId, xyzCoords[0]->data(), xyzCoords[1]->data(), xyzCoords[2]->data() ),
    "ex_get_coord returned an error." );

  //-----------ELEMENT BLOCKS--------------
  // Read the each element block into a separate topology and add it to the domain.
  xdm::RefPtr< xdmGrid::Domain > domain( new xdmGrid::Domain );
  std::vector< int > blockIds( gridParameters.num_elem_blk );
  EXODUS_CHECK( ex_get_ids( fileId, EX_ELEM_BLOCK, &blockIds[0] ), "ex_get_ids returned an error." );
  for( std::vector< int >::const_iterator blockId = blockIds.begin(); blockId != blockIds.end();
    ++blockId ) {
    ExodusString elementType, blockName;
    int numberOfElements, numberOfNodesPerElement, numberOfAttributesPerElement;
    int numberOfEdgesPerElement, numberOfFacesPerElement; // unused here
    EXODUS_CHECK( ex_get_block(
      fileId,
      EX_ELEM_BLOCK,
      *blockId,
      elementType.ptr(),
      &numberOfElements,
      &numberOfNodesPerElement,
      &numberOfEdgesPerElement,
      &numberOfFacesPerElement,
      &numberOfAttributesPerElement ), "ex_get_block returned an error." );
    EXODUS_CHECK( ex_get_name( fileId, EX_ELEM_BLOCK, *blockId, blockName.ptr() ),
      "ex_get_name returned an error." );
    std::vector< int > intConnectivity( numberOfElements * numberOfNodesPerElement );
    EXODUS_CHECK( ex_get_conn( fileId, EX_ELEM_BLOCK, *blockId, &intConnectivity[0], 0, 0 ),
      "ex_get_conn returned an error." );

    // Exodus node numbering starts at 1, so change that to 0. Also, the connections have
    // to be std::size_t for xdm.
    xdm::RefPtr< xdm::VectorStructuredArray< std::size_t > > connectivity(
      new xdm::VectorStructuredArray< std::size_t >( intConnectivity.size() ) );
    std::transform( intConnectivity.begin(), intConnectivity.end(), connectivity->begin(),
      std::bind2nd( std::minus< std::size_t >(), 1 ) );

    // Turn the connectivity into a data item and give it to a topology along with the
    // geometry.
    xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
      connectivity, xdm::primitiveType::kLongUnsignedInt, numberOfElements, numberOfNodesPerElement );
    xdm::RefPtr< xdmGrid::UnstructuredTopology > elementBlock(
      new xdmGrid::UnstructuredTopology() );
    elementBlock->setConnectivity( dataItem );
    elementBlock->setNumberOfCells( numberOfElements );
    elementBlock->setCellType( lookupCellType( numberOfNodesPerElement, elementType.string() ) );
    elementBlock->setNodeOrdering( xdmGrid::NodeOrderingConvention::ExodusII );
    elementBlock->setName( blockName.string() );

    // There is a UniformGrid for each topology... however, they all refer to the same
    // Geometry.
    xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid() );
    grid->setGeometry( geom );
    grid->setTopology( elementBlock );

    // Read the attributes. For element blocks, these are cell-centered values, but there
    // is no way to know here whether they should be interpreted to be scalars,
    // vectors, matrices, etc.
    ExodusString attrNames[ numberOfAttributesPerElement ];
    char* attrNamesPtr[ numberOfAttributesPerElement ];
    std::transform( attrNames, attrNames + numberOfAttributesPerElement, attrNamesPtr,
      std::mem_fun_ref( &ExodusString::ptr ) );
    EXODUS_CHECK( ex_get_attr_names( fileId, EX_ELEM_BLOCK, *blockId, attrNamesPtr ),
      "ex_get_attr_names returned an error." );

    for ( std::size_t attrIndex = 0; attrIndex < numberOfAttributesPerElement; ++attrIndex ) {
      xdm::RefPtr< xdm::VectorStructuredArray< double > > attribute(
        new xdm::VectorStructuredArray< double >( numberOfElements ) );
      EXODUS_CHECK( ex_get_one_attr( fileId, EX_ELEM_BLOCK, *blockId, attrIndex + 1,
        attribute->data() ), "ex_get_one_attr returned an error." );
      xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
        attribute, xdm::primitiveType::kDouble, numberOfElements );
      xdm::RefPtr< xdmGrid::Attribute > attr(
        new xdmGrid::Attribute( xdmGrid::Attribute::kScalar, xdmGrid::Attribute::kCell ) );
      attr->setName( attrNames[ attrIndex ].string() );
      grid->addAttribute( attr );
    }

    domain->addGrid( grid );
  }

  //-------------NODE SETS-----------------
  // Read node sets into polyvertex topologies
  std::vector< int > nodeSetIds( gridParameters.num_node_sets );
  EXODUS_CHECK( ex_get_ids( fileId, EX_NODE_SET, &nodeSetIds[0] ), "ex_get_ids returned an error." );
  for( std::vector< int >::const_iterator nodeSetId = nodeSetIds.begin();
    nodeSetId != nodeSetIds.end(); ++nodeSetId ) {
    int numberOfNodes, numberOfDistributionFactors;
    EXODUS_CHECK( ex_get_set_param(
      fileId,
      EX_NODE_SET,
      *nodeSetId,
      &numberOfNodes,
      &numberOfDistributionFactors ), "ex_get_set_param returned an error." );
    std::vector< int > intNodeList( numberOfNodes );
    EXODUS_CHECK( ex_get_set( fileId, EX_NODE_SET, *nodeSetId, &intNodeList[0], NULL ),
      "ex_get_set returned an error." );

    // Exodus node numbering starts at 1, so change that to 0. Also, the node indices have
    // to be std::size_t for xdm.
    xdm::RefPtr< xdm::VectorStructuredArray< std::size_t > > nodeList(
      new xdm::VectorStructuredArray< std::size_t >( intNodeList.size() ) );
    std::transform( intNodeList.begin(), intNodeList.end(), nodeList->begin(),
      std::bind2nd( std::minus< std::size_t >(), 1 ) );

    // Turn the node list into a data item and give it to a polyvertex topology along with the
    // geometry that everything else uses.
    xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
      nodeList, xdm::primitiveType::kLongUnsignedInt, numberOfNodes );
    xdm::RefPtr< xdmGrid::Polyvertex > polyvertex( new xdmGrid::Polyvertex() );
    polyvertex->setConnectivity( dataItem );
    polyvertex->setNumberOfCells( numberOfNodes );

    xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid() );
    grid->setGeometry( geom );
    grid->setTopology( polyvertex );
    domain->addGrid( grid );
  }



  // TODO: do something with side sets

  // TODO: do something with node number map... maybe read it into polyvertex

  // TODO: do something with element number map

  // TODO: do something with optimized element order map

  // TODO: take care of results

  // TODO: maybe read distribution factors into an attribute

  ex_close( fileId );

  return domain;
}

XDM_EXODUS_NAMESPACE_END

