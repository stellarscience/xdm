//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009-2010 Stellar Science. Government-purpose rights granted.
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
#include <xdmGrid/Grid.hpp>
#include <xdmGrid/Time.hpp>

#include <algorithm>

namespace xdmGrid {

Grid::Grid() :
  xdm::Item(),
  mAttributes(),
  mTime() {
}

Grid::~Grid() {
}

xdm::RefPtr< const Time > Grid::time() const {
  return mTime;
}

void Grid::setTime( xdm::RefPtr< Time > time ) {
  mTime = time;
}

void Grid::addAttribute( xdm::RefPtr< Attribute > attribute ) {
  mAttributes.push_back( attribute );
}

Grid::AttributeIterator Grid::beginAttributes() {
  return mAttributes.begin();
}

Grid::ConstAttributeIterator Grid::beginAttributes() const {
  return mAttributes.begin();
}

Grid::AttributeIterator Grid::endAttributes() {
  return mAttributes.end();
}

Grid::ConstAttributeIterator Grid::endAttributes() const {
  return mAttributes.end();
}

xdm::RefPtr< const Attribute > Grid::attributeByName( const std::string& name ) const {
  for ( std::size_t i = 0; i < mAttributes.size(); ++i ) {
    if ( mAttributes[i]->name() == name ) {
      return mAttributes[i];
    }
  }
  return xdm::RefPtr< const Attribute >();
}

xdm::RefPtr< Attribute > Grid::attributeByName( const std::string& name ) {
  return xdm::const_pointer_cast< Attribute >(
    static_cast< const Grid& >(*this).attributeByName( name )
  );
}

std::size_t Grid::numberOfAttributes() const {
  return mAttributes.size();
}

void Grid::traverse( xdm::ItemVisitor& iv ) {
  if ( mTime.valid() ) {
    mTime->accept( iv );
  }
  std::for_each( mAttributes.begin(), mAttributes.end(), xdm::ApplyVisitor( iv ) );
}

void Grid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Item::writeMetadata( xml );
  xml.setTag( "Grid" );
}

} // namespace xdmGrid

