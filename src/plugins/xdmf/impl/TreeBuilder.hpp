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
#ifndef xdmf_impl_TreeBuilder_hpp
#define xdmf_impl_TreeBuilder_hpp

#include <xdmf/impl/XPathQuery.hpp>

#include <xdm/RefPtr.hpp>

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <vector>

namespace xdm {
class DataItem;
class Item;
class UniformDataItem;
} // namespace xdm

namespace xdmGrid {
class Attribute;
class CollectionGrid;
class Geometry;
class Grid;
class StructuredTopology;
class Time;
class Topology;
class UniformGrid;
} // namespace xdmGrid

namespace xdmf {
namespace impl {

class SharedNodeVector;
class XmlDocumentManager;

/// Class to construct an XDM tree given an XML node that points to the root
/// of an XDMF grid for a single time step.
class TreeBuilder {
public:
  
  /// Construct with the given node representing the root of the time step grid
  /// in the XML document.
  TreeBuilder(
    xdm::RefPtr< XmlDocumentManager > doc,
    xdm::RefPtr< SharedNodeVector > seriesGrids );
  virtual ~TreeBuilder();

  /// Build the tree from the document.
  virtual xdm::RefPtr< xdm::Item > buildTree();

  /// Item build methods. These methods build individual xdm::Item types.
  //@{

  /// Build a grid from an XML node pointing to a Grid element.
  /// @todo Handle Hyperslab, coordinate, and function grids.
  xdm::RefPtr< xdmGrid::Grid > buildGrid( xmlNode * node );

  xdm::RefPtr< xdmGrid::CollectionGrid > buildSpatialCollectionGrid( xmlNode * node );

  xdm::RefPtr<xdmGrid::UniformGrid > buildUniformGrid( xmlNode * node );
  /// Build the Geometry Item corresponding the the given XML node.
  /// @todo Implement XDMF Origin_DxDyDz topology.

  xdm::RefPtr< xdmGrid::Geometry > buildGeometry( xmlNode * node );
  /// Build the Topology Item corresponding to the given XML node.
  /// @todo Implement XDMF unstructured topologies.

  xdm::RefPtr< xdmGrid::Topology > buildTopology( xmlNode * node );

  xdm::RefPtr< xdmGrid::Attribute > buildAttribute( xmlNode * node );

  xdm::RefPtr< xdm::UniformDataItem > buildUniformDataItem( xmlNode * node );

  xdm::RefPtr< xdm::DataItem > buildDataItem( xmlNode * node );

  xdm::RefPtr< xdmGrid::Time > buildTime( xmlNode * node );
  //@}

private:
  TreeBuilder( const TreeBuilder& );
  TreeBuilder& operator=( const TreeBuilder& );

  xdm::RefPtr< XmlDocumentManager > mDoc;
  xdm::RefPtr< SharedNodeVector > mSeriesGrids;
};

} // namespace impl
} // namespace xdmf

#endif // xdmf_impl_TreeBuilder_hpp
