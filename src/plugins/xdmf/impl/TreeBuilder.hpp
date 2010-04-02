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

#include <xdm/RefPtr.hpp>

#include <libxml/tree.h>
#include <libxml/xpath.h>

namespace xdm {
class Item;
class UniformDataItem;
} // namespace xdm

namespace xdmGrid {
class Attribute;
class CollectionGrid;
class Geometry;
class Grid;
class Topology;
class UniformGrid;
} // namespace xdmGrid

namespace xdmf {
namespace impl {

/// Implementation class for building an XDM tree given a parsed and validated
/// XDMF libXml document pointer.
class TreeBuilder {
public:
  
  /// Constructor takes a validated XDMF document. Does not take ownership of
  /// the document.  
  /// @pre document has been validated against the XDMF schema.
  TreeBuilder( xmlDocPtr document );
  virtual ~TreeBuilder();

  /// Build the tree from the document.
  virtual xdm::RefPtr< xdm::Item > buildTree();

  //-- Item builders --//
  xdm::RefPtr< xdm::Item > buildTemporalCollectionGrid( xmlNode * node );
  xdm::RefPtr<xdmGrid::UniformGrid > buildUniformGrid( xmlNode * node );
  xdm::RefPtr< xdmGrid::Geometry > buildGeometry( xmlNode * node );
  xdm::RefPtr< xdmGrid::Topology > buildTopology( xmlNode * node );
  xdm::RefPtr< xdmGrid::Attribute > buildAttribute( xmlNode * node );
  xdm::RefPtr< xdm::UniformDataItem > buildUniformDataItem( xmlNode * node );

private:
  xmlDocPtr mDocument;
  xmlXPathContextPtr mXPathContext;

};

} // namespace impl
} // namespace xdmf

#endif // xdmf_impl_TreeBuilder_hpp
