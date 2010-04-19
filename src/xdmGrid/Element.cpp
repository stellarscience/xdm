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
#include <xdmGrid/Element.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

//-------------------Element-------------------------------------------------------
Element::Element( xdm::RefPtr< ElementSharedImp > imp, std::size_t elementIndex ) :
  ElementBase( imp, elementIndex ) {
}

Element::Element( const Element& copyMe ) :
  ElementBase( copyMe ) {
}

Node Element::node( std::size_t nodeIndex ) {
  return static_cast< const ElementBase& >( *this ).node( nodeIndex ).removeConstness();
}

//-------------------ConstElement--------------------------------------------------
ConstElement::ConstElement( xdm::RefPtr< ElementSharedImp > imp, std::size_t elementIndex ) :
  ElementBase( imp, elementIndex ) {
}

ConstElement::ConstElement( const Element& copyMe ) :
  ElementBase( copyMe ) {
}

ConstElement::ConstElement( const ConstElement& copyMe ) :
  ElementBase( copyMe ) {
}

Element ConstElement::removeConstness() const {
  Element ret( xdm::RefPtr< ElementSharedImp >(), 0 );
  copyReference( *this, ret );
  return ret;
}

//-------------------ElementBase---------------------------------------------------
ElementBase::ElementBase( xdm::RefPtr< ElementSharedImp > imp, std::size_t elementIndex ) :
  mImp( imp ), mIndex( elementIndex ) {
}

ConstNode ElementBase::node( std::size_t nodeIndex ) const {
  return mImp->node( mIndex, nodeIndex );
}

ElementType::Type ElementBase::elementType() const {
  return mImp->elementType( mIndex );
}

ElementBase::ElementBase( const ElementBase& copyMe ) :
  mImp( copyMe.mImp ), mIndex( copyMe.mIndex ) {
}

void ElementBase::copyReference( const ElementBase& source, ElementBase& dest ) {
  dest.mImp = source.mImp;
  dest.mIndex = source.mIndex;
}

XDM_GRID_NAMESPACE_END
