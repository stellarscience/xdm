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

#include <sstream>
#include <string>

#include <cassert>

/// RAII class to manage an XPath query in LibXml and provide simplified access
/// to its results. The lifetime of the underlying query object is tied to the
/// lifetime of the XPathQuery object.
class XPathQuery {
private:
  xmlXPathObjectPtr mXPathObject;
  size_t mSize;
public:
  /// Constructor takes a context, an XML node in which to query, and a query
  /// string.  If the query is invalid, the resulting size of the query will
  /// be 0.
  XPathQuery(
    xmlXPathContextPtr context,
    xmlNode * node,
    const std::string& query ) :
    mXPathObject( NULL )
  {
    xmlNode * oldNode = context->node;
    context->node = node;
    mXPathObject = xmlXPathEvalExpression(
      (xmlChar *)query.c_str(), context );
    if ( mXPathObject && mXPathObject->nodesetval ) {
      mSize = mXPathObject->nodesetval->nodeNr;
    } else {
      mSize = 0;
    }
    context->node = oldNode;
  }

  ~XPathQuery() { xmlXPathFreeObject( mXPathObject ); }

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

#endif // xdmf_impl_XPathQuery_hpp
