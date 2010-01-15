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
#ifndef xdm_XmlObject_hpp
#define xdm_XmlObject_hpp

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <list>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Interface for constructing XML data.  This class exposes simple methods for
/// constructing an XML tree on the fly. It is designed so that the XML output
/// process may be customized by client applications depending on their needs.
class XmlObject : public ReferencedObject {
private:
  std::string mTag;
  typedef std::map< std::string, std::string > AttributeMap;
  AttributeMap mAttributeMap;
  typedef std::vector< std::string > TextContent;
  TextContent mTextContent;
  typedef std::vector< RefPtr< XmlObject > > ChildList;
  ChildList mChildren;

public:

  /// Iterator to expose attributes as a collection of std::pairs.
  typedef AttributeMap::iterator AttributeIterator;
  /// Iterator to expose attributes as a collection of constant std::pairs.
  typedef AttributeMap::const_iterator ConstAttributeIterator;
  /// Iterator to expose children as a collection of reference counted objects.
  typedef ChildList::iterator ChildIterator;
  /// Iterator to expose children as a collection of const reference counted objects.
  typedef ChildList::const_iterator ConstChildIterator;
  /// Iterator to expose the text content as a collection of strings.
  typedef TextContent::iterator TextContentIterator;
  /// Iterator to expose the text content as a collection of const strings.
  typedef TextContent::const_iterator ConstTextContentIterator;

  /// Construct an XmlObject with no tag.
  XmlObject();
  /// Construct an XmlObject with the given tag.
  explicit XmlObject( const std::string& tag );
  virtual ~XmlObject();

  //-- Methods for constructing an XML tree --//

  /// Set the tag for the Object.
  void setTag( const std::string& tag );

  /// Set the attribute with the given name to the given value.  If the
  /// attribute already exists in the XmlObject, it will be overwritten.
  void appendAttribute( const std::string& name, const std::string& value );
  
  /// Append a line of text content to the XmlObject's text.
  void appendContent( const std::string& text );
  
  /// Append another XmlObject as a child of this object.
  void appendChild( XmlObject* child );

  //-- Methods for querying an XML object --//

  /// Get the tag name for this object.
  const std::string& tag() const;

  /// Determine if the object has the specified attribute.
  bool hasAttribute( const std::string& key ) const;

  /// Get the value of the specified attribute.
  const std::string& attribute( const std::string& key ) const;

  /// Get the numbered line of text content.
  const std::string& contentLine( unsigned int line ) const;

  /// Determine if the object has children.
  bool hasChildren() const { return !mChildren.empty(); }

  //-- Iterator interfaces --//

  AttributeIterator beginAttributes() { return mAttributeMap.begin(); }
  ConstAttributeIterator beginAttributes() const { return mAttributeMap.begin(); }
  AttributeIterator endAttributes() { return mAttributeMap.end(); }
  ConstAttributeIterator endAttributes() const { return mAttributeMap.end(); }

  ChildIterator beginChildren() { return mChildren.begin(); }
  ConstChildIterator beginChildren() const { return mChildren.begin(); }
  ChildIterator endChildren() { return mChildren.end(); }
  ConstChildIterator endChildren() const { return mChildren.end(); }

  TextContentIterator beginTextContent() { return mTextContent.begin(); }
  ConstTextContentIterator beginTextContent() const { return mTextContent.begin(); }
  TextContentIterator endTextContent() { return mTextContent.end(); }
  ConstTextContentIterator endTextContent() const { return mTextContent.end(); }

  //-- Methods for outputting an XML object --//

  /// Print the header for the XML object to an ostream.  This is the content
  /// between the <...> of the XML object.
  void printHeader( std::ostream& ostr, int indentLevel = 0 ) const;

  /// Print the text content of the XML object to an ostream.
  void printTextContent( std::ostream& ostr, int indentLevel = 0 ) const;

  /// Print the full body of all of an XmlObject's children to an ostream.
  void printChildren( std::ostream& ostr, int indentLevel = 0 ) const;

  /// Print the footer of the XML object to an ostream.
  void printFooter( std::ostream& ostr, int indentLevel = 0 ) const;

};

/// Helper function for adding a non-string value as an attribute to an XML
/// object.  This function should succeed as long as the input type has the
/// stream insertion operator overloaded.
template< typename T >
void appendAttribute( XmlObject& obj, const std::string& name, const T& value ) {
  std::stringstream ss;
  ss << value;
  obj.appendAttribute( name, ss.str() );
}

/// Non-member function to write an xml object at a given indent level.
std::ostream& writeIndent( std::ostream& ostr, const XmlObject& obj, 
  int indentLevel );

/// Stream insertion operator for an XmlObject.
std::ostream& operator<<( std::ostream& ostr, const XmlObject& obj );

/// Check that two XmlObjects are equal. Objects are considered equal if they
/// have the same tag, attributes, each of their text content lines are equal
/// and in the same order, and all of their descendents are equal in the same
/// way and are in the same order.
bool operator==( const XmlObject& lhs, const XmlObject& rhs );

/// Check that two XmlObjects are not equal. Objects are considered unequal if
/// their tags differ, the attributes they contain or their values differ, any
/// text content lines differ or are out of order, or any descendant is unequal
/// in the same way or in a different order.
bool operator!=( const XmlObject& lhs, const XmlObject& rhs );

XDM_NAMESPACE_END

#endif // xdm_XmlObject_hpp

