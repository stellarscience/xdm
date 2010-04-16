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
#include <xdmGrid/UnstructuredTopology.hpp>

#include <sstream>

//namespace {
//
//class TrivialElementRefImpl : public xdmGrid::ElementSharedImp {
//public:
//  TrivialElementRefImpl(
//    const xdmGrid::ElementType::Type& type,
//    xdm::RefPtr< xdmGrid::Geometry > geometry,
//    std::size_t* nodeIndexArray ) :
//    mType( type ), mGeometry( geometry ), mNodeIds( nodeIndexArray ) {
//  }
//
//  virtual xdmGrid::ConstNode node( std::size_t elementIndex, std::size_t nodeIndex ) const {
//    std::size_t nodeID = mNodeIds[ elementIndex * mType.nodesPerElement() + nodeIndex ];
//    return mGeometry->node( nodeID );
//  }
//
//  virtual xdmGrid::ElementType::Type ElementType( std::size_t elementIndex ) const {
//    return mType;
//  }
//
//private:
//  xdmGrid::ElementType::Type mType;
//  xdm::RefPtr< xdmGrid::Geometry > mGeometry;
//  std::size_t* mNodeIds;
//};
//
//} // anon namespace

XDM_GRID_NAMESPACE_BEGIN

UnstructuredTopology::UnstructuredTopology() :
  Topology(),
  mConnectivity(),
  mElementType( ElementType::Default ),
  mOrdering() {
}

UnstructuredTopology::~UnstructuredTopology() {
}

void UnstructuredTopology::setElementType( const ElementType::Type& type ) {
  mElementType = type;
}

const ElementType::Type& UnstructuredTopology::elementType( std::size_t /* elementIndex */) const {
  return mElementType;
}

void UnstructuredTopology::setNodeOrdering( const NodeOrderingConvention::Type& order )
{
  mOrdering = order;
}

NodeOrderingConvention::Type UnstructuredTopology::nodeOrdering() const
{
  return mOrdering;
}

void UnstructuredTopology::setConnectivity( xdm::RefPtr< xdm::UniformDataItem > connectivity ) {
  mConnectivity = connectivity;
}

void UnstructuredTopology::traverse( xdm::ItemVisitor& iv ) {
  if ( mConnectivity.valid() ) {
    mConnectivity->accept( iv );
  }
}

void UnstructuredTopology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Topology::writeMetadata( xml );

  // Write the number of Elements
  xml.setAttribute( "NumberOfElements", numberOfElements() );

  // Write the Element type
  xml.setAttribute( "ElementType", mElementType.shapeName() );

  // Write the nodes per Element
  xml.setAttribute( "NodesPerElement", mElementType.nodesPerElement() );
}

xdm::RefPtr< xdm::VectorRefImp< std::size_t > > UnstructuredTopology::createVectorImp() {
  return xdm::RefPtr< xdm::VectorRefImp< std::size_t > >(
    new xdm::SingleArrayOfVectorsImp< std::size_t >(
        mConnectivity->typedArray< std::size_t >()->begin(), mElementType.nodesPerElement() ) );
}

XDM_GRID_NAMESPACE_END

