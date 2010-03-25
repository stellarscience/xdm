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

#include <xdmExodus/Blocks.hpp>
#include <xdmExodus/Helpers.hpp>
#include <xdmExodus/Variable.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>
#include <xdmGrid/UnstructuredTopologyConventions.hpp>

#include <xdm/VectorStructuredArray.hpp>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

std::size_t Block::entryGlobalOffset() const {
  return mOffset;
}

void Block::setEntryGlobalOffset( std::size_t offset ) {
  mOffset = offset;
}

std::size_t Block::numberOfEntries() const {
  return topology()->numberOfCells();
}

void Block::addVariable( xdm::RefPtr< Variable > variable ) {
  addAttribute( variable );
}

void Block::readAttributes( int exodusFileId, std::size_t attributesPerEntry ) {
  std::vector< ExodusString > attributeNames( attributesPerEntry );
  char* attributeNamesCharArray[ attributesPerEntry ];
  vectorToCharStarArray( attributeNames, attributeNamesCharArray );
  EXODUS_CALL(
    ex_get_attr_names(
      exodusFileId,
      exodusObjectType(),
      id(),
      attributeNamesCharArray ),
    "Could not read attribute names." );

  for ( std::size_t attributeIndex = 0; attributeIndex < attributesPerEntry; ++attributeIndex ) {
    xdm::RefPtr< xdm::VectorStructuredArray< double > > attribute(
      new xdm::VectorStructuredArray< double >( numberOfEntries() ) );
    EXODUS_CALL(
      ex_get_one_attr(
        exodusFileId,
        exodusObjectType(),
        id(),
        attributeIndex + 1,
        attribute->data() ),
      "Could not read attribute values." );

    xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
      attribute, xdm::primitiveType::kDouble, numberOfEntries() );
    xdm::RefPtr< xdmGrid::Attribute > attr(
      new xdmGrid::Attribute( xdmGrid::Attribute::kScalar, xdmGrid::Attribute::kCell ) );
    attr->setDataItem( dataItem );
    attr->setName( attributeNames[ attributeIndex ].string() );
    addAttribute( attr );
  }

}

void Block::readFromFile(
  int exodusFileId,
  int exodusObjectId,
  std::string blockName,
  xdm::RefPtr< xdmGrid::Geometry > geom,
  std::vector< int >::const_iterator beginTruthTable,
  const std::size_t numberOfVariables,
  const std::vector< ExodusString >& variableNames ) {

  setId( exodusObjectId );
  setName( blockName );
  setGeometry( geom );

  int numberOfEntries = 0;
  int nodesPerEntry = 0;
  int edgesPerEntry = 0;
  int facesPerEntry = 0;
  int attributesPerEntry = 0;
  ExodusString entryType; // e.g. TET4, HEX8
  EXODUS_CALL(
    ex_get_block(
      exodusFileId,
      exodusObjectType(),
      id(),
      entryType.ptr(),
      &numberOfEntries,
      &nodesPerEntry,
      &edgesPerEntry,
      &facesPerEntry,
      &attributesPerEntry ),
    "Could not read block params." );

  std::vector< int > nodeConnections( numberOfEntries * nodesPerEntry );
  std::vector< int > edgeConnections( numberOfEntries * edgesPerEntry );
  std::vector< int > faceConnections( numberOfEntries * facesPerEntry );
  EXODUS_CALL(
    ex_get_conn(
      exodusFileId,
      exodusObjectType(),
      id(),
      &nodeConnections[0],
      &edgeConnections[0],
      &faceConnections[0] ),
    "Could not read connectivity." );

  // TODO: do something with edge/face lists for element blocks.

  xdm::RefPtr< xdm::VectorStructuredArray< std::size_t > > nodeConnectivity(
    new xdm::VectorStructuredArray< std::size_t >( nodeConnections.size() ) );
  convertToZeroBase( nodeConnections, nodeConnectivity->begin() );
  xdm::RefPtr< xdm::UniformDataItem > dataItem = makeDataItem(
    nodeConnectivity, xdm::primitiveType::kLongUnsignedInt, numberOfEntries, nodesPerEntry );
  xdm::RefPtr< xdmGrid::UnstructuredTopology > topo(
    new xdmGrid::UnstructuredTopology() );
  topo->setConnectivity( dataItem );
  topo->setNumberOfCells( numberOfEntries );
  topo->setCellType( xdmGrid::exodusCellType( nodesPerEntry, entryType.string() ) );
  topo->setNodeOrdering( xdmGrid::NodeOrderingConvention::ExodusII );
  setTopology( topo );

  // Read Exodus attributes for this block.
  if ( attributesPerEntry > 0 ) {
    readAttributes( exodusFileId, attributesPerEntry );
  }

  // Read the results variables. For XDM, these will just be additional attributes.
  if ( numberOfVariables > 0 ) {
    setupVariables( beginTruthTable, numberOfVariables, variableNames );

    // Fill the variables with the data from the first time step. If data from another step
    // is needed, the user can call update().
    readTimeStep( exodusFileId, 0 );
  }
}


xdm::RefPtr< Block > blockFactory( int exodusObjectType ) {
  xdm::RefPtr< Block > block;
  switch ( exodusObjectType ) {
    case EX_EDGE_BLOCK:
      block = xdm::makeRefPtr( new EdgeBlock );
      break;
    case EX_FACE_BLOCK:
      block = xdm::makeRefPtr( new FaceBlock );
      break;
    case EX_ELEM_BLOCK:
      block = xdm::makeRefPtr( new ElementBlock );
      break;
    default:
      block.reset();
  }
  return block;
}


XDM_EXODUS_NAMESPACE_END

