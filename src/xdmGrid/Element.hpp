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
#ifndef xdm_Element_hpp
#define xdm_Element_hpp

#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/UnstructuredTopologyConventions.hpp>

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Element;
class ConstElement;
class ElementSharedImp;

class ElementBase
{
public:
  ElementBase( xdm::RefPtr< ElementSharedImp > imp, std::size_t elementIndex );

  /// Get a single const node.
  ConstNode node( std::size_t nodeIndex ) const;

  /// The type of element.
  ElementType::Type elementType() const;

protected:
  ElementBase( const ElementBase& copyMe );

  static void copyReference( const ElementBase& source, ElementBase& dest );

private:
  xdm::RefPtr< ElementSharedImp > mImp;
  std::size_t mIndex;
};

/// A element class that references data in a presumably large dataset. The element acts as
/// self-contained element, but it actually just refers most requests to an implementation
/// class. Thus, the element is lightweight and copying is cheap.
class Element : public ElementBase
{
public:
  Element( xdm::RefPtr< ElementSharedImp > imp, std::size_t elementIndex );
  Element( const Element& copyMe );

  /// Get a single node.
  Node node( std::size_t nodeIndex );

  using ElementBase::node;

private:
  // Referencing a const element with a non-const element is not safe.
  Element( const ConstElement& copyMe );
};

/// Const version of an element.
class ConstElement : public ElementBase
{
public:
  ConstElement( xdm::RefPtr< ElementSharedImp > imp, std::size_t elementIndex );
  ConstElement( const Element& copyMe );
  ConstElement( const ConstElement& copyMe );

  /// This is intended to be a const_cast for elements.
  Element removeConstness() const;

  using ElementBase::node;

private:
  // No non-const node access.
};

/// Shared implementation class for referencing element data that is not stored in a self-contained
/// element structure. All elements that use a given shared implementation should be of the same
/// type, have the same number of nodes, etc. However, that is not required.
class ElementSharedImp : public xdm::ReferencedObject
{
public:
  /// Get a single node.
  virtual ConstNode node( std::size_t elementIndex, std::size_t nodeIndex ) const = 0;

  /// The element type for all elements that this shared implementation refers to.
  virtual ElementType::Type elementType( std::size_t elementIndex ) const = 0;
};

XDM_GRID_NAMESPACE_END

#endif // xdm_Element_hpp
