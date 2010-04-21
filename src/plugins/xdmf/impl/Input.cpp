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
#include <xdmf/impl/Input.hpp>

#include <xdmf/impl/TreeBuilder.hpp>
#include <xdmf/impl/XmlDocumentManager.hpp>

#include <xdmFormat/IoExcept.hpp>

#include <xdm/ThrowMacro.hpp>

namespace xdmf {
namespace impl {

Input::Input(
 xdm::RefPtr< XmlDocumentManager > doc,
 xdm::RefPtr< SharedNodeVector > stepNodes,
 const std::string& xpathExpr ) :
  mDocument( doc ),
  mNodes( stepNodes ),
  mXPathExpr( xpathExpr ) {
}

Input::~Input() {
}

xmlNode * Input::findNode( std::size_t index ) {
  xmlNode * gridNode = nodes()->at( index );
  xmlDoc * doc = document()->get();

  // Find the new XML node within the requested grid context.
  XPathQuery nodeQuery( doc, gridNode, xpathExpr() );
  if ( nodeQuery.size() == 0 ) {
    std::ostringstream ss;
    ss << "XDMF object not found at series index " << index;
    XDM_THROW( xdmFormat::ReadError( ss.str() ) );
  }
  return nodeQuery.node( 0 );
}

}
}
