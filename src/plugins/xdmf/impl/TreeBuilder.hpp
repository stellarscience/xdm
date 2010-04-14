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
#define xdmf_impl_TreeBuildter_hpp

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

/// A list of XML nodes.
typedef std::vector< xmlNode * > NodeList;

template< typename ItemT >
class InputItem : public ItemT {
public:
  InputItem() : ItemT() {}
  virtual ~InputItem() {}

  void setXPathExpr( const std::string& xpathExpr ) { mXPathExpr = xpathExpr; }
  const std::string& xpathExpr() const { return mXPathExpr; }

private:
  std::string mXPathExpr;
};

/// Class to construct an XDM tree given an XML node that points to the root
/// of an XDMF grid for a single time step.
class TreeBuilder {
public:
  
  /// Construct with the given node representing the root of the time step grid
  /// in the XML document.
  TreeBuilder( xmlNode * gridRoot );
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

  /// Configure a StructuredTopology given an XML node with XDMF Topology content.
  void configureStructuredTopology( xdmGrid::StructuredTopology& t, xmlNode * content );

  /// Configure a UniformDataItem given an XML node with XDMF UniformDataItem content.
  void configureUniformDataItem( xdm::UniformDataItem& item, xmlNode * content );

private:
  xmlNode * mGridRoot;
  xmlDoc * mDocument;
  xmlXPathContextPtr mXPathContext;
};

} // namespace impl
} // namespace xdmf

#endif // xdmf_impl_TreeBuilder_hpp
