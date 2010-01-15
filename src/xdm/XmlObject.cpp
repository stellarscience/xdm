#include <xdm/XmlObject.hpp>

XDM_NAMESPACE_BEGIN

namespace {

  struct PrintAttributeValuePair {
    std::ostream& mOstr;
    char mDelimiter;
    
    PrintAttributeValuePair( std::ostream& ostr, char delimiter ) :
      mOstr( ostr ), mDelimiter( delimiter ) {
    }
    
    void operator()( const std::pair< std::string, std::string >& p ) {
      mOstr << p.first << "=" << p.second << mDelimiter;
    }
  };

  struct PrintXmlObjectFunctor {
    std::ostream& mOstr;
    int mIndent;
    PrintXmlObjectFunctor( std::ostream& ostr, int indent ) :
      mOstr( ostr ), mIndent( indent ) {}
    void operator()( const XmlObject* obj ) {
      obj->printHeader( mOstr, mIndent );
      obj->printBody( mOstr, mIndent + 1 );
      obj->printFooter( mOstr, mIndent );
    }
  };

  void indentLine( std::ostream& ostr, int level ) {
    for ( int i = 0; i < level; ++i ) {
      ostr << "  ";
    }
  }

} // namespace anon

XmlObject::XmlObject( const std::string& tag ) :
  mTag( tag ),
  mAttributeMap(),
  mTextContent() {
}

XmlObject::~XmlObject() {
}

void XmlObject::appendAttribute( 
  const std::string& name, 
  const std::string& value ) {
  mAttributeMap[name] = value;
}

void XmlObject::appendContent( const std::string& text ) {
  std::stringstream ss( text );
  ss << std::endl;
  mTextContent.push_back( ss.str() );
}

void XmlObject::appendChild( XmlObject* child ) {
  mChildren.push_back( child );
}

void XmlObject::printHeader( std::ostream& ostr, int indentLevel ) const {
  indentLine( ostr, indentLevel );
  ostr << "<" << mTag << " ";
  std::for_each( mAttributeMap.begin(), mAttributeMap.end(), 
    PrintAttributeValuePair( ostr, ' ' ) );
  ostr << ">" << std::endl;
}

void XmlObject::printBody( std::ostream& ostr, int indentLevel ) const {
  // print my text first
  TextContent::const_iterator line;
  for ( line = mTextContent.begin(); line != mTextContent.end(); ++line ) {
    indentLine( ostr, indentLevel );
    ostr << *line << std::endl;
  }
  
  // now print the children.
  std::for_each( mChildren.begin(), mChildren.end(), 
    PrintXmlObjectFunctor( ostr, indentLevel ) );
}

void XmlObject::printFooter( std::ostream& ostr, int indentLevel ) const {
  indentLine( ostr, indentLevel );
  ostr << "</" << mTag << ">" << std::endl;
}

std::ostream& operator<<( std::ostream& ostr, const XmlObject& obj ) {
  PrintXmlObjectFunctor print( ostr, 0 );
  print( &obj );
}


XDM_NAMESPACE_END

