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
#ifndef xdmf_impl_XPathQuery_hpp
#define xdmf_impl_XPathQuery_hpp

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <deque>
#include <sstream>
#include <string>

#include <cassert>

namespace xdmf {
namespace impl {

/// Class to represent a node within a path. Contains a std::pair with a node
/// and the child number within an XPath query it is in it's parent element.
/// For example in an XML hierarchy that looks like:
///
/// @code
/// <root>
///   <child name='fred'/>
///   <another-child/>
///   <child name='jeff'/>
/// </root>
/// @endcode
///
/// And an XPath query @code /root/child @endcode, the 'child' element with
/// name 'fred' will appear as a pair with (node, 0) and the 'child' element
/// with name 'jeff' will appear as a pair with (node, 1) since that is the
/// order they are encountered in within the /root/child XPathQuery.
typedef std::deque< std::pair< xmlNode *, size_t > > NodePath;

/// Push a node-index pair onto the back of a NodePath. The index is 0-based.
inline void pushNode( xmlNode * node, size_t index, NodePath& nodePath ) {
  nodePath.push_back( std::make_pair( node, index ) );
}

/// Pop a node-index pair from the back of a NodePath.
inline void popNode( NodePath& nodePath ) {
  nodePath.pop_back();
}

/// Make an XPath query from a NodePath. Since NodePaths are 0-based, but XPath
/// enumeration predicates are 1-based, this function adds 1 to every index in
/// the NodePath.
inline std::string makeXPathQuery( const NodePath& nodePath ) {
  std::ostringstream result;
  NodePath::const_iterator node = nodePath.begin();
  while( node != nodePath.end() ) {
    // Add 1 to the index since XPath predicates are 1-based.
    result << node->first->name << '[' << node->second + 1 << ']';
    if ( ++node != nodePath.end() ) result << '/';
  }
  return result.str();
}

/// RAII class to manage an XPath query in LibXml and provide simplified access
/// to its results. The lifetime of the underlying query object is tied to the
/// lifetime of the XPathQuery object.
class XPathQuery {
private:
  xmlXPathContextPtr mXPathContext;
  xmlXPathObjectPtr mXPathObject;
  size_t mSize;

public:
  /// Constructor takes a context, an XML node in which to query, and a query
  /// string.  If the query is invalid, the resulting size of the query will
  /// be 0.
  XPathQuery(
    xmlDoc * document,
    xmlNode * node,
    const std::string& query ) :
    mXPathObject( NULL )
  {
    mXPathContext = xmlXPathNewContext( document );
    mXPathContext->node = node;
    mXPathObject = xmlXPathEvalExpression(
      (xmlChar *)query.c_str(), mXPathContext );
    if ( mXPathObject && mXPathObject->nodesetval ) {
      mSize = mXPathObject->nodesetval->nodeNr;
    } else {
      mSize = 0;
    }
  }

  ~XPathQuery() {
    xmlXPathFreeObject( mXPathObject );
    xmlXPathFreeContext( mXPathContext );
  }

  size_t size() const {
    return mSize;
  }

  std::string textValue( size_t i ) const {
    assert( i < mSize );
    xmlNode* node = mXPathObject->nodesetval->nodeTab[i];
    char * text = (char*)xmlNodeGetContent( node );
    if ( text ) {
      std::string result( text );
      free( text );
      return result;
    }
    return std::string();
  }

  template< typename T > bool value( size_t i, T& outValue ) {
    std::istringstream ss( textValue( i ) );
    ss >> outValue;
    if ( !ss.bad() ) {
      return true;
    } else {
      return false;
    }
  }

  template< typename T > T getValue( size_t i, T defaultValue ) {
    T result;
    if ( value( i, result ) ) {
      return result;
    } else {
      return defaultValue;
    }
  }

  xmlNode * node( size_t i ) {
    assert( i < mSize );
    return mXPathObject->nodesetval->nodeTab[i];
  }
};

/// Find a path to an ancestor (tail recursive implementation).
inline void findPathToAncestor(
  xmlDoc * doc,
  xmlNode * descendant,
  xmlNode * ancestor,
  NodePath& accumulator )
{
  if ( descendant == (xmlNode*)doc || descendant == NULL ) {
    accumulator.clear();
    return;
  }

  if ( descendant == ancestor ) {
    return;
  }

  // Determine which child of it's parent the descendant is
  std::string queryString( (char *)descendant->name );
  XPathQuery child( doc, descendant->parent, queryString );
  size_t index = 0;
  while( child.node(index) != descendant ) {
    index++;
  }
  pushNode( descendant, index, accumulator );

  // Recurse to find the parent's path.
  findPathToAncestor(
    doc,
    descendant->parent,
    ancestor,
    accumulator );
}

/// Find a path to an ancestor (entry point).
inline NodePath findPathToAncestor( xmlDoc * doc, xmlNode * descendant, xmlNode * ancestor ) {
  NodePath result;
  findPathToAncestor( doc, descendant, ancestor, result );
  return result;
}

} // namespace impl
} // namespace xdmf

#endif // xdmf_impl_XPathQuery_hpp
