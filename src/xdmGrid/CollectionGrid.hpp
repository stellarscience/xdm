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
#ifndef xdmGrid_CollectionGrid_hpp
#define xdmGrid_CollectionGrid_hpp

#include <xdmGrid/Grid.hpp>

#include <xdm/Forward.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Grid type that contains other Grids.  The collection can be spatial or
/// temporal, that is a collection of multiple grids in different locations, or
/// a collection of grids representing the simulation at different points in
/// time.
class CollectionGrid : public Grid {
public:

  /// Enumeration of collection type.  Spatial or temporal.
  enum CollectionType {
    kSpatial = 0,
    kTemporal
  };

  explicit CollectionGrid( CollectionType type = kSpatial );
  virtual ~CollectionGrid();

  XDM_META_ITEM( CollectionGrid );

  /// Set the collection type for this grid to spatial or temporal.
  /// @see CollectionType
  void setType( CollectionType t );
  CollectionType type() const;

  /// Append a referenced grid with the corresponding element indices.
  /// @param grid The grid that will be referenced by this collection.
  /// @param elementIndices An integer array containing the element indices for the elements
  ///        that are referenced in grid. If this array is omitted, then all
  ///        elements in the corresponding grid will be referenced, in default order.
  void appendGrid(
    xdm::RefPtr< Grid > grid,
    xdm::RefPtr< xdm::UniformDataItem > elementIndices = xdm::RefPtr< xdm::UniformDataItem >() );

  /// Create an attribute with the proper dimensions. This will initialize an Attribute holding
  /// onto a UniformDataItem that has the correct array shape. The attribute will be attached
  /// to the grid. However, the UniformDataItem will not have any data (MemoryAdapter); that
  /// is left up to the user to supply.
  /// @note Node-centered attributes are not allowed for a CollectionGrid because the collection
  ///       is a collection of elements rather than a collection of geometries.
  virtual xdm::RefPtr< xdmGrid::Attribute > createAttribute(
    Attribute::Center center,
    Attribute::Type type,
    const std::string& name,
    xdm::primitiveType::Value dataType );

  /// Since the CollectionGrid can contain subsets of many different topologies,
  /// this function returns the total number of referenced elements. This is an
  /// expensive call because it recomputes the total number of elements at each
  /// function call.
  virtual std::size_t numberOfElements() const;

  /// Get an element by index. All elements are const in that they only have const functions.
  virtual Element element( const std::size_t& elementIndex ) const;

  /// Definition of visitor traversal.
  virtual void traverse( xdm::ItemVisitor& iv );

  /// Definition of metadata output.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  std::vector< xdm::RefPtr< xdmGrid::Grid > > mGrids;
  std::vector< xdm::RefPtr< xdm::UniformDataItem > > mArrays;
  std::vector< std::size_t > mElementOffsets;
  CollectionType mType;

  std::pair< Grid*, std::size_t > findGrid( const std::size_t& elementIndex ) const;
  void updateOffsets();
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_CollectionGrid_hpp

