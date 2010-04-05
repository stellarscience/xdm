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
#include <xdmExodus/Sets.hpp>
#include <xdmExodus/Helpers.hpp>
#include <xdmExodus/Variable.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

std::size_t Set::numberOfEntries() const {
  return numberOfCells();
}

void Set::addVariable( xdm::RefPtr< Variable > variable ) {
  Object::addVariable( variable );
  addAttribute( variable );
}

void Set::writeToFile( int exodusFileId, int* variableTruthTable ) {

}

void Set::setDistributionFactors( xdm::RefPtr< xdm::UniformDataItem > factors ) {
  mDistributionFactors = factors;
}

xdm::RefPtr< xdm::UniformDataItem > Set::distributionFactors() {
  return mDistributionFactors;
}

void Set::setExtraFlags( xdm::RefPtr< xdm::UniformDataItem > extraFlags ) {
  mExtraFlags = extraFlags;
}

xdm::RefPtr< xdm::UniformDataItem > Set::extraFlags() {
  return mExtraFlags;
}

xdm::RefPtr< Set > setFactory( int exodusObjectType ) {
  switch ( exodusObjectType ) {
    case EX_NODE_SET:
      return xdm::makeRefPtr( new NodeSet );
    case EX_EDGE_SET:
      return xdm::makeRefPtr( new EdgeSet );
    case EX_FACE_SET:
      return xdm::makeRefPtr( new FaceSet );
    case EX_SIDE_SET:
      return xdm::makeRefPtr( new SideSet );
    case EX_ELEM_SET:
      return xdm::makeRefPtr( new ElementSet );
  }
  return xdm::RefPtr< Set >( NULL );
}

XDM_EXODUS_NAMESPACE_END
