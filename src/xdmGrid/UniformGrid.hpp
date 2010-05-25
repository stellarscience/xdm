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

#include <xdmGrid/Geometry.hpp>  // For the Node and ConstNode typedefs
#include <xdmGrid/Forward.hpp>
#include <xdmGrid/Grid.hpp>

#include <xdm/Forward.hpp>
#include <xdm/PrimitiveType.hpp>

#include <string>
#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

// Code Review Matter (open): UniformStructuredGrid
// As a user of this class, I had the thought we might want to add something
// like UniformStructuredGrid, which uses covariant return types to return
// concrete Geometry and Topology objects, instead of their base class
// pointers.  The reason is I found myself needing a dynamic cast to recover
// the static type of the Geometry and Topology I originally put in.  For
// the most commonly used combinations of Geometry and Topology sub-types,
// it seems like a welcome addition to the interface.
// -- Curtis Cooper on 2010-05-20
// One wouldn't be able to use xdm::RefPtr as part of the covariant return,
// so one would have to use the intrusive property of ReferencedObject
// to recover the RefPtr.
// -- K. R. Walker on 2010-05-21

/// Grid type containing the actual geometry and topology for a grid.  This is a
/// terminal grid node that contains the geometric and topological properties of
/// a Grid along with any attributes defined on the grid.
class UniformGrid : public Grid {
public:
  UniformGrid();
  virtual ~UniformGrid();

  XDM_META_ITEM( UniformGrid );

  // Code Review Matter (open): geo and topo
  // -- K. R. Walker on 2010-05-21

  void setGeometry( xdm::RefPtr< Geometry > geo );
  xdm::RefPtr< Geometry > geometry();
  xdm::RefPtr< const Geometry > geometry() const;

  void setTopology( xdm::RefPtr< Topology > topo );
  xdm::RefPtr< Topology > topology();
  xdm::RefPtr< const Topology > topology() const;

  /// Create an attribute with the proper dimensions. This will initialize an Attribute holding
  /// onto a UniformDataItem that has the correct array shape. The attribute will be attached
  /// to the grid. However, the UniformDataItem will not have any data (MemoryAdapter); that
  /// is left up to the user to supply.
  virtual xdm::RefPtr< xdmGrid::Attribute > createAttribute(
    Attribute::Center center,
    Attribute::Type type,
    const std::string& name,
    xdm::primitiveType::Value dataType );

  /// Get the total number of elements in this grid.
  virtual std::size_t numberOfElements() const;

  /// Get an element by index. All elements are const in that they only have const functions.
  virtual Element element( const std::size_t& elementIndex ) const;

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

private:
  xdm::RefPtr< Geometry > mGeometry;
  xdm::RefPtr< Topology > mTopology;
  xdm::RefPtr< ElementSharedConnectivityLookup > mElementImp;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_UniformGrid_hpp

