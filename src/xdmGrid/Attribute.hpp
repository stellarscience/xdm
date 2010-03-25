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
#ifndef xdmGrid_Attribute_hpp
#define xdmGrid_Attribute_hpp

#include <xdm/Item.hpp>
#include <xdm/ItemVisitor.hpp>
#include <xdm/RefPtr.hpp>

#include <xdmGrid/NamespaceMacro.hpp>
#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN
  class UniformDataItem;
XDM_NAMESPACE_END

XDM_GRID_NAMESPACE_BEGIN

class Attribute : public xdm::Item {
public:
  /// Enumeration of attribute types.
  enum Type {
    kScalar = 0,
    kVector,
    kTensor,
    kTensor6,
    kMatrix
  };

  /// Enumeration of attribute centering.
  enum Center {
    kNode = 0,
    kEdge,
    kFace,
    kCell,
    kGrid
  };

  Attribute( Type t = kScalar, Center c = kNode );
  virtual ~Attribute();

  XDM_META_ITEM( Attribute );

  /// Set the attribute's data structure and type.
  void setDataItem( xdm::RefPtr< xdm::UniformDataItem > item );
  /// Get the attribute's data structure and type.
  xdm::RefPtr< xdm::UniformDataItem > dataItem();
  /// Get the attribute's const data structure and type.
  xdm::RefPtr< const xdm::UniformDataItem > dataItem() const;

  /// Get the type of the attribute.
  Type dataType() const;

  /// Get the centering.
  Center centering() const;

  virtual void traverse( xdm::ItemVisitor& iv );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  Type mType;
  Center mCenter;
  xdm::RefPtr< xdm::UniformDataItem > mDataItem;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Attribute_hpp

