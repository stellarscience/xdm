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
#include <xdmGrid/Cell.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

//-------------------Cell-------------------------------------------------------
Cell::Cell( xdm::RefPtr< CellSharedImp > imp, std::size_t cellIndex ) :
  CellBase( imp, cellIndex ) {
}

Cell::Cell( const Cell& copyMe ) :
  CellBase( copyMe ) {
}

Node Cell::node( std::size_t nodeIndex ) {
  return static_cast< const CellBase& >( *this ).node( nodeIndex ).removeConstness();
}

//-------------------ConstCell--------------------------------------------------
ConstCell::ConstCell( xdm::RefPtr< CellSharedImp > imp, std::size_t cellIndex ) :
  CellBase( imp, cellIndex ) {
}

ConstCell::ConstCell( const Cell& copyMe ) :
  CellBase( copyMe ) {
}

ConstCell::ConstCell( const ConstCell& copyMe ) :
  CellBase( copyMe ) {
}

Cell ConstCell::removeConstness() const {
  Cell ret( xdm::RefPtr< CellSharedImp >(), 0 );
  copyReference( *this, ret );
  return ret;
}

//-------------------CellBase---------------------------------------------------
CellBase::CellBase( xdm::RefPtr< CellSharedImp > imp, std::size_t cellIndex ) :
  mImp( imp ), mIndex( cellIndex ) {
}

ConstNode CellBase::node( std::size_t nodeIndex ) const {
  return mImp->node( mIndex, nodeIndex );
}

CellType::Type CellBase::cellType() const {
  return mImp->cellType( mIndex );
}

CellBase::CellBase( const CellBase& copyMe ) :
  mImp( copyMe.mImp ), mIndex( copyMe.mIndex ) {
}

void CellBase::copyReference( const CellBase& source, CellBase& dest ) {
  dest.mImp = source.mImp;
  dest.mIndex = source.mIndex;
}

XDM_GRID_NAMESPACE_END
