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
#ifndef xdmGrid_UniformGrid_hpp
#define xdmGrid_UniformGrid_hpp

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Forward.hpp>
#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/Grid.hpp>

#include <xdm/Forward.hpp>
#include <xdm/ObjectCompositionMixin.hpp>
#include <xdm/PrimitiveType.hpp>

#include <string>
#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Grid type containing the actual geometry and topology for a grid.  This is a
/// terminal grid node that contains the geometric and topological properties of
/// a Grid along with any attributes defined on the grid.
class UniformGrid :
  public Grid,
  public xdm::ObjectCompositionMixin< Attribute > {
public:
  UniformGrid();
  virtual ~UniformGrid();

  XDM_META_ITEM( UniformGrid );

  void setGeometry( xdm::RefPtr< Geometry > geo );
  xdm::RefPtr< Geometry > geometry();
  xdm::RefPtr< const Geometry > geometry() const;

  void setTopology( xdm::RefPtr< Topology > topo );
  xdm::RefPtr< Topology > topology();
  xdm::RefPtr< const Topology > topology() const;

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

  /// Get an element by index.
  Element element( std::size_t elementIndex );
  /// Get a const element by index.
  ConstElement element( std::size_t elementIndex ) const;

  /// Get a node by index. This does not take connectivity into consideration.
  /// This means that the input index is the index into the geometry directly,
  /// not into the grid with a node order specified by the topology.
  Node node( std::size_t nodeIndex );
  /// Get a node by index. This does not take connectivity into consideration.
  /// This means that the input index is the index into the geometry directly,
  /// not into the grid with a node order specified by the topology.
  ConstNode node( std::size_t nodeIndex ) const;

  /// Redefinition of visitor traversal from xdm::Item.
  virtual void traverse( xdm::ItemVisitor& iv );

  /// Redefinition of metadata from Grid.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

protected:
  /// If a class inherits from this class, it can set the shared imp for ease
  /// of implementation.
  void setElementSharedImp( xdm::RefPtr< ElementSharedImp > elementImp );

private:
  xdm::RefPtr< Geometry > mGeometry;
  xdm::RefPtr< Topology > mTopology;
  xdm::RefPtr< ElementSharedImp > mElementImp;
};

/// Construct an attribute on a uniform grid with the given center.
///
/// This non-member function will construct the attribute definition with a
/// corresponding UniformDataItem to hold the attribute data.  The attribute is
/// not automatically added to the grid, it is up to clients to do that.
xdm::RefPtr< xdmGrid::Attribute >
createAttribute(
  xdm::RefPtr< const UniformGrid > grid,
  Attribute::Center center,
  Attribute::Type type,
  const std::string& name,
  xdm::primitiveType::Value dataType );

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_UniformGrid_hpp

