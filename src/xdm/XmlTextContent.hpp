#ifndef xdm_XmlTextContent_hpp
#define xdm_XmlTextContent_hpp

#include <xdm/RefPtr.hpp>
#include <xdm/XmlObject.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Wrapper object that exposes only the text content of an XmlObject.
class XmlTextContent {
protected:
  RefPtr< XmlObject > mXml;

public:
  /// Constructor takes an XmlObject to wrap.
  XmlTextContent( RefPtr< XmlObject > xml ) : mXml( xml ) {}
  ~XmlTextContent() {}

  /// Append a line of content to the XmlObject.
  void appendContentLine( const std::string& line ) { 
    mXml->appendContent( line );
  }

  /// Query a line of content from the XmlObject.
  const std::string& contentLine( unsigned int line ) const {
    return mXml->contentLine( line );
  }
};

XDM_NAMESPACE_END

#endif // xdm_XmlTextContent_hpp

