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
#ifndef xdmf_impl_Input_hpp
#define xdmf_impl_Input_hpp

#include <xdm/RefPtr.hpp>

#include <libxml/tree.h>

#include <string>

namespace xdmf {
namespace impl {

class TreeBuilder;
class XmlDocumentManager;
class SharedNodeVector;

/// Class containing a shared document and a shared list of nodes to hold on to
/// for stepping an XDMF item in a series.
/// @param doc The document containing the XDMF description.
/// @param nodes The nodes containing the description of the item at each step.
class Input {
public:
  Input(
    xdm::RefPtr< XmlDocumentManager > doc,
    xdm::RefPtr< SharedNodeVector > stepNodes,
    const std::string& xpathExpr );
  virtual ~Input();

  /// Read implementation to build an item from a node.
  virtual void read( xmlNode * node, TreeBuilder& builder ) = 0;

  /// Find the node for this object at the given series index.
  xmlNode * findNode( std::size_t index );

  xdm::RefPtr< XmlDocumentManager > document() { return mDocument; }
  xdm::RefPtr< SharedNodeVector > nodes() { return mNodes; }
  const std::string& xpathExpr() const { return mXPathExpr; }

private:
  Input( const Input& );
  Input& operator=( const Input& );

  xdm::RefPtr< XmlDocumentManager > mDocument;
  xdm::RefPtr< SharedNodeVector > mNodes;
  std::string mXPathExpr;
};

} // namespace impl
} // namespace xdmf

#endif // xdmf_impl_Input_hpp
