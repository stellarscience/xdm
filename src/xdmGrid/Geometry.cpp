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
#include <xdmGrid/Geometry.hpp>

#include <algorithm>

XDM_GRID_NAMESPACE_BEGIN

Geometry::Geometry( unsigned int dimension ) :
  mNumberOfNodes( 0 ),
  mDimension( dimension ),
  mSharedVectorImp() {
}

Geometry::~Geometry() {
}

void Geometry::setDimension( unsigned int dimension ) {
  mDimension = dimension;
}

unsigned int Geometry::dimension() const {
  return mDimension;
}

void Geometry::setNumberOfNodes( std::size_t n )
{
  mNumberOfNodes = n;
}

std::size_t Geometry::numberOfNodes() const
{
  return mNumberOfNodes;
}

ConstNode Geometry::node( std::size_t nodeIndex ) const
{
  if ( !mSharedVectorImp ) {
    Geometry& mutableThis = const_cast< Geometry& >( *this );
    mutableThis.mSharedVectorImp = mutableThis.createVectorImp();
  }
  return ConstNode( mSharedVectorImp, nodeIndex );
}

Node Geometry::node( std::size_t nodeIndex )
{
  if ( !mSharedVectorImp ) {
    mSharedVectorImp = this->createVectorImp();
  }
  return Node( mSharedVectorImp, nodeIndex );
}

void Geometry::traverse( xdm::ItemVisitor& iv ) {
  std::for_each( begin(), end(), xdm::ApplyVisitor( iv ) );
}

void Geometry::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::Item::writeMetadata( xml );
  xml.setTag( "Geometry" );
}

XDM_GRID_NAMESPACE_END

