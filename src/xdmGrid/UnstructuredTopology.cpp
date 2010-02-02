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
#include <xdmGrid/Element.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>

#include <sstream>

XDM_GRID_NAMESPACE_BEGIN

UnstructuredTopology::UnstructuredTopology() :
  Topology(),
  mNumberOfElements(),
  mNodesPerElement(),
  mConnectivity() {
}

UnstructuredTopology::~UnstructuredTopology() {
}

void UnstructuredTopology::setNumberOfElements( std::size_t numberOfElements ) {
  mNumberOfElements = numberOfElements;
}

std::size_t UnstructuredTopology::numberOfElements() const {
  return mNumberOfElements;
}

void UnstructuredTopology::setNodesPerElement( std::size_t nodesPerElement ) {
  mNodesPerElement = nodesPerElement;
}

std::size_t UnstructuredTopology::nodesPerElement() const {
  return mNodesPerElement;
}

void UnstructuredTopology::setElementShape( const ElementShape::Type& shape )
{
  mShape = shape;
}

ElementShape::Type UnstructuredTopology::elementShape() const
{
  return mShape;
}

void UnstructuredTopology::setNodeOrdering( const NodeOrderingConvention::Type& order )
{
  mOrdering = order;
}

NodeOrderingConvention::Type UnstructuredTopology::nodeOrdering() const
{
  return mOrdering;
}

Element UnstructuredTopology::element( std::size_t elementIndex )
{
  return Element( mElementSharedImpl, elementIndex );
}

const Element UnstructuredTopology::element( std::size_t elementIndex ) const
{
  return Element( mElementSharedImpl, elementIndex );
}

void UnstructuredTopology::setConnectivity( xdm::DataItem* connectivity ) {
  mConnectivity = connectivity;
}

void UnstructuredTopology::traverse( xdm::ItemVisitor& iv ) {
  if ( mConnectivity.valid() ) {
    mConnectivity->accept( iv );
  }
}

void UnstructuredTopology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Topology::writeMetadata( xml );

  // Write the number of elements
  xml.setAttribute( "NumberOfElements", mNumberOfElements );

  // Write the nodes per element
  xml.setAttribute( "NodesPerElement", mNodesPerElement );
}

XDM_GRID_NAMESPACE_END

