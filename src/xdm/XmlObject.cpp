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
#include <xdm/XmlObject.hpp>

#include <xdm/Algorithm.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

namespace {

  struct PrintAttributeValuePair {
    std::ostream& mOstr;
    char mDelimiter;
    
    PrintAttributeValuePair( std::ostream& ostr, char delimiter ) :
      mOstr( ostr ), mDelimiter( delimiter ) {
    }
    
    void operator()( const std::pair< std::string, std::string >& p ) {
      mOstr << mDelimiter << p.first << "='" << p.second << "'";
    }
  };

  struct PrintXmlObjectFunctor {
    std::ostream& mOstr;
    int mIndent;
    PrintXmlObjectFunctor( std::ostream& ostr, int indent ) :
      mOstr( ostr ), mIndent( indent ) {}
    void operator()( const XmlObject* obj ) {
      obj->printHeader( mOstr, mIndent );
      obj->printTextContent( mOstr, mIndent + 1 );
      obj->printChildren( mOstr, mIndent + 1 );
      obj->printFooter( mOstr, mIndent );
    }
  };

  void indentLine( std::ostream& ostr, int level ) {
    for ( int i = 0; i < level; ++i ) {
      ostr << "  ";
    }
  }

} // namespace

XmlObject::XmlObject() :
  mTag(),
  mAttributeMap(),
  mTextContent() {
}

XmlObject::XmlObject( const std::string& tag ) :
  mTag( tag ),
  mAttributeMap(),
  mTextContent() {
}

XmlObject::~XmlObject() {
}

//-----------------------------------------------------------------------------
// Tree Construction
//-----------------------------------------------------------------------------

void XmlObject::setTag( const std::string& tag ) {
  mTag = tag;
}

void XmlObject::appendAttribute( 
  const std::string& name, 
  const std::string& value ) {
  mAttributeMap[name] = value;
}

void XmlObject::appendContent( const std::string& text ) {
  mTextContent.push_back( text );
}

void XmlObject::appendChild( XmlObject* child ) {
  mChildren.push_back( child );
}

//-----------------------------------------------------------------------------
// Queries
//-----------------------------------------------------------------------------
const std::string& XmlObject::tag() const {
  return mTag;
}

bool XmlObject::hasAttribute( const std::string& key ) const {
  AttributeMap::const_iterator it = mAttributeMap.find( key );
  return ( it != mAttributeMap.end() );
}

const std::string& XmlObject::attribute( const std::string& key ) const {
  AttributeMap::const_iterator it = mAttributeMap.find( key );
  if ( it == mAttributeMap.end() ) {
    std::stringstream msg( "No such attribute: " );
    msg << key;
    XDM_THROW( std::runtime_error( msg.str() ) );
  }
  return it->second;
}

const std::string& XmlObject::contentLine( unsigned int line ) const {
  if ( line >= mTextContent.size() ) {
    std::stringstream msg( "Content line out of range: " );
    msg << line;
    XDM_THROW( std::runtime_error( msg.str() ) );
  }
  return mTextContent[line];
}

//-----------------------------------------------------------------------------
// Formatting
//-----------------------------------------------------------------------------
void XmlObject::printHeader( std::ostream& ostr, int indentLevel ) const {
  // raise an exception if the XmlObject has an empty tag.
  if ( mTag.empty() ) {
    XDM_THROW( std::runtime_error( "XmlObject is empty" ) );
  }
  indentLine( ostr, indentLevel );
  ostr << "<" << mTag;
  std::for_each( mAttributeMap.begin(), mAttributeMap.end(), 
    PrintAttributeValuePair( ostr, ' ' ) );
  ostr << ">" << std::endl;
}

void XmlObject::printTextContent( std::ostream& ostr, int indentLevel ) const {
  TextContent::const_iterator line;
  for ( line = mTextContent.begin(); line != mTextContent.end(); ++line ) {
    indentLine( ostr, indentLevel );
    ostr << *line << std::endl;
  }
}

void XmlObject::printChildren( std::ostream& ostr, int indentLevel ) const {
  std::for_each( mChildren.begin(), mChildren.end(), 
    PrintXmlObjectFunctor( ostr, indentLevel ) );
}

void XmlObject::printFooter( std::ostream& ostr, int indentLevel ) const {
  if ( mTag.empty() ) { 
    XDM_THROW( std::runtime_error( "XmlObject is empty" ) );
  }
  indentLine( ostr, indentLevel );
  ostr << "</" << mTag << ">" << std::endl;
}

std::ostream& writeIndent( std::ostream& ostr, const XmlObject& obj,
  int indentLevel ) {
  PrintXmlObjectFunctor print( ostr, indentLevel );
  print( &obj );
  return ostr;
}

std::ostream& operator<<( std::ostream& ostr, const XmlObject& obj ) {
  return writeIndent( ostr, obj, 0 );
}

bool operator==( const XmlObject& lhs, const XmlObject& rhs ) {
  if ( lhs.tag() != rhs.tag() ) {
    return false;
  }

  // attribute order doesn't matter.
  if ( !unorderedCollectionsEqual(
    lhs.beginAttributes(), lhs.endAttributes(),
    rhs.beginAttributes(), rhs.endAttributes() ) ) {
    return false;
  }

  // Text line order matters.
  if ( !orderedCollectionsEqual(
    lhs.beginTextContent(), lhs.endTextContent(),
    rhs.beginTextContent(), rhs.endTextContent() ) ) {
    return false;
  }

  // Child order matters. Check the children are equal recursively.
  if ( !orderedCollectionsEqual(
    lhs.beginChildren(), lhs.endChildren(),
    rhs.beginChildren(), rhs.endChildren(),
    EqualPointerValue< RefPtr< XmlObject > > () ) ) {
    return false;
  }

  // It is known that the attributes, text content, and children are equal.
  return true;
}

bool operator!=( const XmlObject& lhs, const XmlObject& rhs ) {
  return !( lhs == rhs );
}

XDM_NAMESPACE_END

