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
#include <xdmGrid/Attribute.hpp>

#include <xdm/UniformDataItem.hpp>

#include <algorithm>
#include <map>
#include <string>

namespace xdmGrid {

namespace {
  struct AttributeTypeMapping :
    public std::map< Attribute::Type, std::string > {
    AttributeTypeMapping() {
      using std::make_pair;
      insert( make_pair( Attribute::kScalar, "Scalar" ) );
      insert( make_pair( Attribute::kVector, "Vector" ) );
      insert( make_pair( Attribute::kTensor, "Tensor" ) );
      insert( make_pair( Attribute::kTensor6, "Tensor6" ) );
      insert( make_pair( Attribute::kMatrix, "Matrix" ) );
    }
  };
  static AttributeTypeMapping sAttributeTypeMapping;

  struct AttributeCenterMapping :
    public std::map< Attribute::Center, std::string > {
    AttributeCenterMapping() {
      using std::make_pair;
      insert( make_pair( Attribute::kNode, "Node" ) );
      insert( make_pair( Attribute::kEdge, "Edge" ) );
      insert( make_pair( Attribute::kFace, "Face" ) );
      insert( make_pair( Attribute::kElement, "Cell" ) );
      insert( make_pair( Attribute::kGrid, "Grid" ) );
    }
  };
  static AttributeCenterMapping sAttributeCenterMapping;
} // namespace anon

Attribute::Attribute( Type t, Center c ) :
  mType( t ),
  mCenter( c ) {
}

Attribute::~Attribute() {
}

void Attribute::setDataItem( xdm::RefPtr< xdm::UniformDataItem > item ) {
  mDataItem = item;
}

xdm::RefPtr< xdm::UniformDataItem > Attribute::dataItem() {
  return mDataItem;
}

xdm::RefPtr< const xdm::UniformDataItem > Attribute::dataItem() const {
  return mDataItem;
}

Attribute::Type Attribute::dataType() const {
  return mType;
}

void Attribute::setDataType( Type type ) {
  mType = type;
}

Attribute::Center Attribute::centering() const {
  return mCenter;
}

void Attribute::setCentering( Center center ) {
  mCenter = center;
}

void Attribute::traverse( xdm::ItemVisitor& iv ) {
  mDataItem->accept( iv );
}

void Attribute::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::Item::writeMetadata( xml );

  xml.setTag( "Attribute" );

  xml.setAttribute( "AttributeType", sAttributeTypeMapping[ mType ] );
  xml.setAttribute( "Center", sAttributeCenterMapping[ mCenter] );
}

} // namespace xdmGrid

