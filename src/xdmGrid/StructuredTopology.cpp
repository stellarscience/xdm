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
  mNodes.clear();
  switch( mShape.rank() ) {
    case 2:
      for ( std::size_t x = 0; x < mShape[0]; ++x ) {
        for ( std::size_t y = 0; y < mShape[1]; ++y ) {
          std::size_t xNodes = mShape[0] + 1;
          mNodes.push_back( ( x + 0 ) + ( y + 0 ) * xNodes );
          mNodes.push_back( ( x + 1 ) + ( y + 0 ) * xNodes );
          mNodes.push_back( ( x + 1 ) + ( y + 1 ) * xNodes );
          mNodes.push_back( ( x + 0 ) + ( y + 1 ) * xNodes );
        }
      }
      break;
    case 3:
      for ( std::size_t x = 0; x < mShape[0]; ++x ) {
        for ( std::size_t y = 0; y < mShape[1]; ++y ) {
          for ( std::size_t z = 0; z < mShape[2]; ++z ) {
            std::size_t xNodes = mShape[0] + 1;
            std::size_t yNodes = mShape[1] + 1;
            mNodes.push_back( ( x + 0 ) + ( y + 0 ) * xNodes + ( z + 0 ) * xNodes * yNodes );
            mNodes.push_back( ( x + 1 ) + ( y + 0 ) * xNodes + ( z + 0 ) * xNodes * yNodes );
            mNodes.push_back( ( x + 1 ) + ( y + 1 ) * xNodes + ( z + 0 ) * xNodes * yNodes );
            mNodes.push_back( ( x + 0 ) + ( y + 1 ) * xNodes + ( z + 0 ) * xNodes * yNodes );
            mNodes.push_back( ( x + 0 ) + ( y + 0 ) * xNodes + ( z + 1 ) * xNodes * yNodes );
            mNodes.push_back( ( x + 1 ) + ( y + 0 ) * xNodes + ( z + 1 ) * xNodes * yNodes );
            mNodes.push_back( ( x + 1 ) + ( y + 1 ) * xNodes + ( z + 1 ) * xNodes * yNodes );
            mNodes.push_back( ( x + 0 ) + ( y + 1 ) * xNodes + ( z + 1 ) * xNodes * yNodes );
          }
        }
      }
      break;
    default:
      std::stringstream ss;
      ss << "The rank of the DataShape represents an unknown structured mesh topology (something"
        " other than 2D or 3D). The rank was " << mShape.rank();
      XDM_THROW( std::runtime_error( ss.str() ) );
  }

  return xdm::RefPtr< xdm::VectorRefImp< std::size_t > >(
    new xdm::SingleArrayOfVectorsImp< std::size_t >( &mNodes[0], mCellType.nodesPerCell() ) );
}

XDM_GRID_NAMESPACE_END

