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
#include <xdmGrid/StructuredTopology.hpp>

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <xdm/ThrowMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

template< typename T >
class SelfContainedVec : public xdm::VectorRefImp< T > {
public:
  SelfContainedVec( const std::vector< T >& data ) :
    mData( data ) {}

  virtual const T& at( std::size_t /* baseIndex */, std::size_t i ) const {
    return mData[i];
  }

  virtual std::size_t size() const { return mData.size(); }
private:
  std::vector< T > mData;
};

StructuredTopology::StructuredTopology() :
  Topology(),
  mShape(),
  mCellType( CellType::Default ) {
}

StructuredTopology::~StructuredTopology() {
}

NodeOrderingConvention::Type StructuredTopology::nodeOrdering() const {
  return NodeOrderingConvention::ExodusII;
}

void StructuredTopology::setShape( const xdm::DataShape<>& shape ) {
  mShape = shape;
  std::size_t rank = mShape.rank();
  switch( rank ) {
    case 2:
      mCellType = CellType::Quad;
      break;
    case 3:
      mCellType = CellType::Hex;
      break;
    default:
      std::stringstream ss;
      ss << "The rank of the DataShape represents an unknown structured mesh topology (something"
        " other than 2D or 3D). The rank was " << rank;
      XDM_THROW( std::runtime_error( ss.str() ) );
  }
}

const xdm::DataShape<>& StructuredTopology::shape() const {
  return mShape;
}

ConstCellConnectivity StructuredTopology::cellConnections( std::size_t cellIndex ) const {
  std::size_t dimension = mShape.rank();
  std::vector< std::size_t > nodeIndices;
  switch ( dimension ) {
    case 2: {
      nodeIndices.resize( 4 );
      std::size_t xCell = cellIndex % mShape[0];
      std::size_t yCell = cellIndex / mShape[0];
      std::size_t xNodes = mShape[0] + 1;
      nodeIndices[0] = ( xCell + 0 ) + ( yCell + 0 ) * xNodes;
      nodeIndices[1] = ( xCell + 1 ) + ( yCell + 0 ) * xNodes;
      nodeIndices[2] = ( xCell + 1 ) + ( yCell + 1 ) * xNodes;
      nodeIndices[3] = ( xCell + 0 ) + ( yCell + 1 ) * xNodes;
      break;
    }
    case 3: {
      nodeIndices.resize( 8 );
      std::size_t xCell = cellIndex % mShape[0];
      std::size_t yCell = cellIndex / mShape[0];
      std::size_t zCell = cellIndex / mShape[0] / mShape[1];
      std::size_t xNodes = mShape[0] + 1;
      std::size_t yNodes = mShape[1] + 1;
      nodeIndices[0] = ( xCell + 0 ) + ( yCell + 0 ) * xNodes + ( zCell + 0 ) * xNodes * yNodes;
      nodeIndices[1] = ( xCell + 1 ) + ( yCell + 0 ) * xNodes + ( zCell + 0 ) * xNodes * yNodes;
      nodeIndices[2] = ( xCell + 1 ) + ( yCell + 1 ) * xNodes + ( zCell + 0 ) * xNodes * yNodes;
      nodeIndices[3] = ( xCell + 0 ) + ( yCell + 1 ) * xNodes + ( zCell + 0 ) * xNodes * yNodes;
      nodeIndices[4] = ( xCell + 0 ) + ( yCell + 0 ) * xNodes + ( zCell + 1 ) * xNodes * yNodes;
      nodeIndices[5] = ( xCell + 1 ) + ( yCell + 0 ) * xNodes + ( zCell + 1 ) * xNodes * yNodes;
      nodeIndices[6] = ( xCell + 1 ) + ( yCell + 1 ) * xNodes + ( zCell + 1 ) * xNodes * yNodes;
      nodeIndices[7] = ( xCell + 0 ) + ( yCell + 1 ) * xNodes + ( zCell + 1 ) * xNodes * yNodes;
      break;
    }
    default: {
      std::stringstream ss;
      ss << "The rank of the DataShape represents an unknown structured mesh topology (something"
        " other than 2D or 3D). The rank was " << dimension;
      XDM_THROW( std::runtime_error( ss.str() ) );
    }
  }

  xdm::RefPtr< xdm::VectorRefImp< std::size_t > > vecImp(
    new SelfContainedVec< std::size_t >( nodeIndices ) );
  return ConstCellConnectivity( vecImp, 0 );
}

const CellType::Type& StructuredTopology::cellType( std::size_t /* cellIndex */ ) const {
  return mCellType;
}

void StructuredTopology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Topology::writeMetadata( xml );

  // write the shape of the topology
  std::stringstream ss;
  ss << mShape;
  xml.setAttribute( "Dimensions", ss.str() );
}

xdm::RefPtr< xdm::VectorRefImp< std::size_t > > StructuredTopology::createVectorImp() {
  // This is not needed since cellConnections() is overridden by this class.
  return xdm::RefPtr< xdm::VectorRefImp< std::size_t > >( NULL ); }

XDM_GRID_NAMESPACE_END

