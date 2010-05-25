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
#ifndef xdm_Grid_hpp
#define xdm_Grid_hpp

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Forward.hpp>

#include <xdm/Forward.hpp>
#include <xdm/Item.hpp>
#include <xdm/ItemVisitor.hpp> // ItemVisitor is used in the XDM_META_ITEM macro.
#include <xdm/PrimitiveType.hpp>

#include <string>
#include <vector>

namespace xdmGrid {

/// @brief Base class for all Items representing grid data.
///
/// A Grid is a collection of Elements, Nodes, and Attributes representing
/// quantities on those elements or nodes. An Element is a region in space, a
/// node is a vertex on the boundary of an Element, and an Attribute is some
/// quantity (vector or scalar) defined on either the elements or nodes of the
/// Grid. For example, in a thermal simulation using a tetrahedral mesh, the
/// Elements are the tetrahedra, the Nodes are the vertices of the Tetrahedra,
/// and an Attribute is temperature.
///
/// @see Element
/// @see Attribute
class Grid : public xdm::Item {
public:
  Grid();
  virtual ~Grid();

  XDM_META_ITEM( Grid );

  // Code Review Matter (open): Returning by value
  // Did you consider returning by "const xdm::RefPtr< Type >&"? This would still result
  // in a copy if the client code was not changed, but may be good practice
  // to avoid unnecessary copies.
  // -- Todd on 2010-05-21

  // Code Review Matter (open): Access to private data
  // Is there ever a time (pun intended) when it is appropriate for a client to
  // have direct access to this member data?
  // Did you consider only providing const access and the setter?
  // -- Todd on 2010-05-21

  xdm::RefPtr< Time > time();
  xdm::RefPtr< const Time > time() const;
  void setTime( xdm::RefPtr< Time > time );

  // Code Review Matter (open): Passing by value
  // Did you consider passing by "const xdm::RefPtr< Type >&"?
  // This would eliminate one unnecessary copy operation.
  // -- Todd on 2010-05-21

  /// Add an attribute definition to the grid.
  void addAttribute( xdm::RefPtr< Attribute > attribute );
 
  /// Iterator to access attributes.
  typedef std::vector< xdm::RefPtr< Attribute > >::iterator AttributeIterator;
  /// Const Iterator to access attributes.
  typedef std::vector< xdm::RefPtr< Attribute > >::const_iterator ConstAttributeIterator;

  /// Get an iterator pointing to the beginning of the Grid attributes.
  AttributeIterator beginAttributes();
  /// Get a constant iterator pointing to the beginning of the Grid attributes.
  ConstAttributeIterator beginAttributes() const;
  /// Get an iterator pointing to the end of the Grid attributes;
  AttributeIterator endAttributes();
  /// Get a const iterator pointing to the end of the Grid attributes.
  ConstAttributeIterator endAttributes() const;

  /// Get an attribute by name.
  xdm::RefPtr< Attribute > attributeByName( const std::string& name );
  /// Get a const attribute by name.
  xdm::RefPtr< const Attribute > attributeByName( const std::string& name ) const;

  /// Get the number of attributes.
  std::size_t numberOfAttributes() const;

  // Code Review Matter (open): Forward Attribute
  // If the Attribute::Center and Attribute::Type enums were moved out of
  // class Attribute, would it be possible to forward class Attribute?
  // -- K. R. Walker on 2010-05-21

  // Code Review Matter (open): xdm::primitiveType::Value
  // Would xdm::LanguagePrimitive::Type be a more descriptive name that might
  // more closely follow the coding standards?
  // -- K. R. Walker on 2010-05-21

  /// Create an attribute with the proper dimensions. This will initialize an Attribute holding
  /// onto a UniformDataItem that has the correct array shape. The attribute will be attached
  /// to the grid. However, the UniformDataItem will not have any data (MemoryAdapter); that
  /// is left up to the user to supply.
  virtual xdm::RefPtr< xdmGrid::Attribute > createAttribute(
    Attribute::Center center,
    Attribute::Type type,
    const std::string& name,
    xdm::primitiveType::Value dataType ) = 0;

  /// Get the total number of elements in this grid.
  virtual std::size_t numberOfElements() const = 0;

  /// Get an element by index. All elements are const in that they only have const functions.
  virtual Element element( const std::size_t& elementIndex ) const = 0;

  virtual void traverse( xdm::ItemVisitor& iv );

  /// Write grid metadata.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

public:
  std::vector< xdm::RefPtr< Attribute > > mAttributes;
  xdm::RefPtr< Time > mTime;
};

} // namespace xdmGrid

#endif // xdm_Grid_hpp

