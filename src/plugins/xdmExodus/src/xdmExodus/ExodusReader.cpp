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

#include <xdmGrid/CollectionGrid.hpp>
#include <xdmGrid/MultiArrayGeometry.hpp>
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

XDM_EXODUS_NAMESPACE_BEGIN

namespace {

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

  ex_init_params gridParameters;
  int error = ex_get_init_ext( fileId, &gridParameters );

  // Read the nodes. For Exodus, there is only ever one set of nodes that all element blocks
  // refer to.
  std::vector< xdm::RefPtr< xdm::StructuredArray > > xyzCoords( 3 );
  xdm::RefPtr< xdmGrid::MultiArrayGeometry > geom(
    new xdmGrid::MultiArrayGeometry( gridParameters.num_dim ) );
  for( std::size_t dim = 0; dim < gridParameters.num_dim; ++dim ) {
    xyzCoords[ dim ] = new xdm::VectorStructuredArray< double >( gridParameters.num_nodes );
    xdm::DataShape<> datasetShape(1);
    datasetShape[0] = xyzCoords[ dim ]->size();
    xdm::RefPtr< xdm::UniformDataItem > dataItem(
      xdm::makeRefPtr( new xdm::UniformDataItem( xdm::primitiveType::kDouble, datasetShape ) ) );
    dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( xyzCoords[ dim ] ) ) );
    geom->setCoordinateValues( dim, dataItem );
  }
  error = ex_get_coord( fileId, xyzCoords[0]->data(), xyzCoords[1]->data(), xyzCoords[2]->data() );

  // Read the each element block into a separate topology and add it to the main collection.
  xdm::RefPtr< xdmGrid::CollectionGrid > collection(
    new xdmGrid::CollectionGrid( xdmGrid::CollectionGrid::kSpatial ) );
  std::vector< int > blockIds( gridParameters.num_elem_blk );
  error = ex_get_ids( fileId, EX_ELEM_BLOCK, &blockIds[0] );
  for( std::vector< int >::const_iterator blockId = blockIds.begin(); blockId != blockIds.end();
    ++blockId ) {
    char elementType[ MAX_STR_LENGTH + 1 ];
    int numberOfElements, numberOfNodesPerElement, numberOfAttributesPerElement;
    int numberOfEdgesPerElement, numberOfFacesPerElement; // unused here
    error = ex_get_block(
      fileId,
      EX_ELEM_BLOCK,
      *blockId,
      elementType,
      &numberOfElements,
      &numberOfNodesPerElement,
      &numberOfEdgesPerElement,
      &numberOfFacesPerElement,
      &numberOfAttributesPerElement );
    std::vector< int > intConnectivity( numberOfElements * numberOfNodesPerElement );
    error = ex_get_conn( fileId, EX_ELEM_BLOCK, *blockId, &intConnectivity[0], 0, 0 );

    // Exodus numbering starts at 1, so change that to 0. Also, the connections have
    // to be std::size_t for xdm.
    xdm::RefPtr< xdm::VectorStructuredArray< std::size_t > > connectivity(
      new xdm::VectorStructuredArray< std::size_t >( intConnectivity.size() ) );
    std::transform( intConnectivity.begin(), intConnectivity.end(), connectivity->begin(),
      std::bind2nd( std::minus< std::size_t >(), 1 ) );

    // Turn the connectivity into a data item and give it to a topology along with the
    // geometry.
    xdm::DataShape<> datasetShape(1);
    datasetShape[0] = connectivity->size();
    xdm::RefPtr< xdm::UniformDataItem > dataItem( xdm::makeRefPtr(
      new xdm::UniformDataItem( xdm::primitiveType::kLongUnsignedInt, datasetShape ) ) );
    dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( connectivity ) ) );
    xdm::RefPtr< xdmGrid::UnstructuredTopology > elementBlock(
      new xdmGrid::UnstructuredTopology() );
    elementBlock->setConnectivity( dataItem );
    elementBlock->setNumberOfCells( numberOfElements );
    elementBlock->setCellType( lookupCellType( numberOfNodesPerElement, elementType ) );
    elementBlock->setNodeOrdering( xdmGrid::NodeOrderingConvention::ExodusII );

    // There is a UniformGrid for each topology... however, they all refer to the same
    // Geometry.
    xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid() );
    grid->setGeometry( geom );
    grid->setTopology( elementBlock );
    collection->appendChild( grid );
  }

  ex_close( fileId );

  return collection;
}

XDM_EXODUS_NAMESPACE_END

