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
#ifndef xdmGrid_ReferencingGrid_hpp
#define xdmGrid_ReferencingGrid_hpp

#include <xdmGrid/UniformGrid.hpp>

#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class ReferencingGrid :
  public UniformGrid,
  public xdm::ObjectCompositionMixin< UniformGrid >,
  public xdm::ObjectCompositionMixin< xdm::UniformDataItem > {
public:
  ReferencingGrid();
  virtual ~ReferencingGrid();

  XDM_META_ITEM( ReferencingGrid );

  /// Since the ReferencingGrid can contain subsets of many different topologies,
  /// this function returns the total number of referenced elements. This is an
  /// expensive call because it recomputes the total number of elements at each
  /// function call.
  std::size_t numberOfElements() const;

  /// Set the number of referenced grids.
  void setNumberOfReferencedGrids( const std::size_t n );

  /// Append a reference grid with the corresponding element indices.
  /// @param grid The grid that will be referenced.
  /// @param elementIndices An integer array containing the element indices for the elements
  ///        that are referenced in grid. If this array is omitted, then all
  ///        elements in the corresponding grid will be referenced, in default order.
  void appendReferenceGrid(
    xdm::RefPtr< UniformGrid > grid,
    xdm::RefPtr< xdm::UniformDataItem > elementIndices = xdm::RefPtr< xdm::UniformDataItem >() );

  /// Set a reference grid with the corresponding element indices.
  /// @param grid The grid that will be referenced.
  /// @param elementIndices An integer array containing the element indices for the elements
  ///        that are referenced in grid.
  /// @param gridIndex The index of the reference grid. Note that the order of the
  ///        grids directly affects the ordering of the refernced elements. For example,
  ///        calling element( 0 ) returns an element from the first grid, with whatever index
  ///        is in the elementIndices corresponding to the first grid.
  /// @pre setNumberOfReferencedGrids() must have been called and gridIndex must be less
  ///      than the number of grids.
  void setReferenceGrid(
    const std::size_t gridIndex,
    xdm::RefPtr< UniformGrid > grid,
    xdm::RefPtr< xdm::UniformDataItem > elementIndices = xdm::RefPtr< xdm::UniformDataItem >() );

  /// Redefinition of visitor traversal from xdm::Item.
  virtual void traverse( xdm::ItemVisitor& iv );

  /// Redefinition of metadata from Grid.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  /// These are not relevant for ReferenceGrids.
  void setGeometry( xdm::RefPtr< Geometry > geo );
  xdm::RefPtr< Geometry > geometry();
  xdm::RefPtr< const Geometry > geometry() const;
  void setTopology( xdm::RefPtr< Topology > topo );
  xdm::RefPtr< Topology > topology();
  xdm::RefPtr< const Topology > topology() const;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_ReferencingGrid_hpp

