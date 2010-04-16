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
#include <xdmGrid/Topology.hpp>

#include <algorithm>

XDM_GRID_NAMESPACE_BEGIN

Topology::Topology() :
  xdm::Item(),
  mNumberOfElements( 0 ),
  mSharedVectorImp() {
}

Topology::~Topology() {
}

void Topology::setNumberOfElements( std::size_t numberOfElements ) {
  mNumberOfElements = numberOfElements;
}

std::size_t Topology::numberOfElements() const {
  return mNumberOfElements;
}

ConstElementConnectivity Topology::elementConnections( std::size_t elementIndex ) const {
  if ( ! mSharedVectorImp ) {
    Topology& mutableThis = const_cast< Topology& >( *this );
    mutableThis.mSharedVectorImp = mutableThis.createVectorImp();
  }
  return ConstElementConnectivity( mSharedVectorImp, elementIndex );
}

void Topology::traverse( xdm::ItemVisitor& iv ) {
  std::for_each( begin(), end(), xdm::ApplyVisitor( iv ) );
}

void Topology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::Item::writeMetadata( xml );
  xml.setTag( "Topology" );
}

XDM_GRID_NAMESPACE_END
