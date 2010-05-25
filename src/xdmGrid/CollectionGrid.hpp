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

/// A grid type that contains other Grids. The CollectionGrid refers to specific
/// subsets of elements in other grids while retaining the Grid interface for
/// accessing elements as if they are part of a single, uniform grid. This grid
/// can also refer to subsets of faces or edges that lie on elements in other
/// grids. Thus, the CollectionGrid is a superset of the XDMF concept of a
/// CollectionGrid.
class CollectionGrid : public Grid {
public:

  // Code Review Matter (open): nested enumeration
  // Did you consider moving enumerations outside of the classes to remove
  // any need for a full #include when only a forward declaration is needed?
  // -- K. R. Walker on 2010-05-21

  /// Enumeration of collection type.  Spatial or temporal.
  enum CollectionType {
    kSpatial = 0,
    kTemporal
  };

  explicit CollectionGrid( CollectionType type = kSpatial );
  virtual ~CollectionGrid();

  XDM_META_ITEM( CollectionGrid );

  // Code Review Matter (open): locked type
  // If this grid is set up as kSpatial and has data added, would the following
  // methods make it possible to make a nonsensical object?
  // In other words, what are the invariants of objects of this class?
  // -- K. R. Walker on 2010-05-21

  /// Set the collection type for this grid to spatial or temporal.
  /// @see CollectionType
  void setType( CollectionType t );
  CollectionType type() const;

  // Code Review Matter (open): duplicates?
  // Can a grid be added to the collection more than once?
  // -- K. R. Walker on 2010-05-21

  /// Append a grid, referencing all of the elements in the grid.
  /// @param grid The grid that will be referenced by this collection.
  void appendGrid( xdm::RefPtr< Grid > grid );

  // Code Review Matter (open): should => must?
  // What is the behavior when the indicies are not unique?
  // Is this a precondition that, when violated, invokes undefined behavior?
  // -- K. R. Walker on 2010-05-21

  /// Append a grid, referencing only a subset of the elements.
  /// @param grid The grid that will be referenced by this collection.
  /// @param elementIndices An integer array containing the element indices for the elements
  ///        that are referenced in grid. The indices should be unique so that each element
  ///        is only referenced once.
  void appendGrid(
    xdm::RefPtr< Grid > grid,
    xdm::RefPtr< xdm::UniformDataItem > elementIndices );

  /// Append a grid, referencing some of the faces on some of the elements.
  /// @param grid The grid that will be referenced by this collection.
  /// @param elementIndices An integer array containing the element indices for the elements
  ///        that are referenced in grid. These indicies need not be unique because multiple
  ///        faces from a single element may be referenced.
  /// @param faceIndices An integer array containing the indices of the faces corresponding
  ///        to the elements in elementIndices. The ordering of faces on elements is specified
  ///        by the node ordering convention. @see xdmGrid::ElementTopology
  void appendGridFaces(
    xdm::RefPtr< Grid > grid,
    xdm::RefPtr< xdm::UniformDataItem > elementIndices,
    xdm::RefPtr< xdm::UniformDataItem > faceIndices );

  /// Append a grid, referencing some of the edges on some of the elements.
  /// @param grid The grid that will be referenced by this collection.
  /// @param elementIndices An integer array containing the element indices for the elements
  ///        that are referenced in grid. These indicies need not be unique because multiple
  ///        edges from a single element may be referenced.
  /// @param edgeIndices An integer array containing the indices of the edges corresponding
  ///        to the elements in elementIndices. The ordering of edges on elements is specified
  ///        by the node ordering convention. @see xdmGrid::ElementTopology
  void appendGridEdges(
    xdm::RefPtr< Grid > grid,
    xdm::RefPtr< xdm::UniformDataItem > elementIndices,
    xdm::RefPtr< xdm::UniformDataItem > edgeIndices );

  // Code Review Matter (open): center != Attribute::Center::kNode
  // Should this be documented as a @pre condition?
  // What happens if this is violated since it is a change in behavior
  // from the base class? Null Attribute? How might a client determine why
  // an attribue wasn't created when working with the base class interface?
  // -- K. R. Walker on 2010-05-21

  /// Create an attribute with the proper dimensions. This will initialize an Attribute holding
  /// onto a UniformDataItem that has the correct array shape. The attribute will be attached
  /// to the grid. However, the UniformDataItem will not have any data (MemoryAdapter); that
  /// is left up to the user to supply.
  /// @note Node-centered attributes are not allowed for a CollectionGrid because the collection
  ///       is a collection of elements rather than a collection of geometries.
  virtual xdm::RefPtr< Attribute > createAttribute(
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
  enum ReferenceType {
    kElement,
    kFace,
    kEdge
  };

  // The following vectors are parallel vectors. For each grid that is referenced by this
  // collection, there will be corresponding entries in mGrids, mElementIndices,
  // mFaceEdgeIndices, mElementOffsets, and mReferenceTypes.

  // The grids whose elements are referenced by this collection.
  std::vector< xdm::RefPtr< xdmGrid::Grid > > mGrids;

  // For each grid in mGrid, we need to store a list of element indices for the subset of
  // elements that will be used from that grid, e.g. if we want elements 2, 4, and 6 from
  // the 2nd grid, then mElementIndices[1] == { 2, 4, 6 }
  std::vector< xdm::RefPtr< xdm::UniformDataItem > > mElementIndices;

  // If the corresponding grid in mGrid is being used to reference lower-dimensional portions
  // of elements, then we store those indices here, e.g. if we want faces on the 2nd grid, and
  // those faces are faces 1, 2, and 3 on elements 2, 2, and 4, then we have:
  //   mElementIndices[1] == { 2, 2, 4 }
  //   mFaceEdgeIndices[1] == { 1, 2, 3 }
  std::vector< xdm::RefPtr< xdm::UniformDataItem > > mFaceEdgeIndices;

  // For fast lookup of elements, we store the cumulative element index for each grid. For
  // example, if we want 3 elements from the 1st grid, 5 elements from the 2nd grid, and
  // 2 elements from the 3rd grid, then we have:
  //   mElementOffsets == { 3, 8, 10 }
  mutable std::vector< std::size_t > mElementOffsets;

  // For each grid in mGrids, mReferenceTypes indicates whether we are accessing elements,
  // faces, or edges on the elements in that grid. This allows the use of heterogeneous
  // grids in a collection, e.g. we can add a grid of triangles while referencing the
  // triangles directly as elements and then also add a grid of tetrahedra while referencing
  // faces on the tets (which are triangles); thus, the collection grid always refers to
  // triangles.
  std::vector< ReferenceType > mReferenceTypes;

  // This is going away, right Will?
  CollectionType mType;

  std::pair< std::size_t, std::size_t > findGrid( const std::size_t& elementIndex ) const;
  void updateOffsets() const;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_CollectionGrid_hpp

