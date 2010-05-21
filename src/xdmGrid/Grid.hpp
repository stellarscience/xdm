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
// Code Review Matter (open): Foward declaration
// ItemVisitor does not seem to be used. Would a foraward declaration be sufficient?
// -- Todd on 21 May 2010
#include <xdm/ItemVisitor.hpp>
#include <xdm/PrimitiveType.hpp>

#include <string>
#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

// Code Review Matter (open): Optional namespace
// It appears that namespaces are optional in the XDM code base; however, the code
// uses them explicitly in many places.
// Did you concider adding macros for when the name space is required rather than
// only where it wraps code? For example:
//
// #define XDM_NS xdm::
// class Grid : public XDM_NS Item { // I am not positive that would even work.
//
// If the library must support platforms that don't support namespaces, and since this could
// possibley pollute the global namesapce with names that are likely to conflict (like Item),
// did you consider using a library prefixes instead (XdmItem)?
// -- Todd on 21 May 2010

XDM_GRID_NAMESPACE_BEGIN

// Code Review 05-24/2010
//
// Curtis: Is it necessary for all value passing to occur through xdm::RefPtr,
// or can we be returning by reference in many (or most) cases?
//
// Curtis: Did you consider providing documentation on what this class is 
// intended to do?  Or maybe just a URL to external docs would be nice.  The
// main reason I ask this is, based on my limited usage of XDM so far, I don't
// understand why Grid has both attributes and elements, and when to use one
// instead of the other.

class Grid : public xdm::Item {
public:
  Grid();
  virtual ~Grid();

  XDM_META_ITEM( Grid );

  // Code Review Matter (open): Returning by value
  // Did you consider returning by "const xdm::RefPtr< type >&" this would still result
  // in a copy if the cleint code was not change, but may be a good standard practice
  // to avoid unnesisary copies.
  // -- Todd on 21 May 2010

  // Code Review Matter (open): Access to private data
  // Is there ever at time (pun intended) when it is appropriate for a client to
  // have direct access to this member data?
  // Did you consider only providing const access and the setter?
  // -- Todd on 21 May 2010

  xdm::RefPtr< Time > time();
  xdm::RefPtr< const Time > time() const;
  void setTime( xdm::RefPtr< Time > time );

  // Code Review Matter (open): Passing by value
  // Did you consider passing by const "const xdm::RefPtr< type >&" this would eliminate
  // one unnessiary copy opeartion.
  // -- Todd on 21 May 2010

  /// Add an attribute definition to the grid.
  void addAttribute( xdm::RefPtr< Attribute > attribute );
  /// Get an attribute by its index.
  xdm::RefPtr< Attribute > attributeByIndex( std::size_t index );
  /// Get a const attribute by its index.
  xdm::RefPtr< const Attribute > attributeByIndex( std::size_t index ) const;
  /// Get an attribute by name.
  xdm::RefPtr< Attribute > attributeByName( const std::string& name );
  /// Get a const attribute by name.
  xdm::RefPtr< const Attribute > attributeByName( const std::string& name ) const;

  /// Get the number of attributes.
  std::size_t numberOfAttributes() const;

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

XDM_GRID_NAMESPACE_END

#endif // xdm_Grid_hpp

