#ifndef xdm_XmlMetadataWrapper_hpp
#define xdm_XmlMetadataWrapper_hpp

#include <xdm/RefPtr.hpp>
#include <xdm/XmlObject.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Wrapper to expose only the metadata portion of the XmlObject interface.
/// This allows us to control access to the children of an XmlObject, which we
/// generally only want to expose to certain classes. For instance, when
/// allowing items to specify their XML representation, we want to control how
/// children are added via visitors.  This interface allows us to pass only the
/// portion of the XmlObject required to specify metadata, not aggregate
/// objects.
class XmlMetadataWrapper {
private:
  RefPtr< XmlObject > mXml;
public:
  /// Constructor takes an XmlObject to wrap.
  XmlMetadataWrapper( RefPtr< XmlObject > xml ) : mXml( xml ) {}
  ~XmlMetadataWrapper() {}

  /// Set the tag for the underlying XmlObject.
  void setTag( const std::string& name ) { mXml->setTag( name ); }
  /// Get the tag for the underlying XmlOBject.
  const std::string& tag() const { return mXml->tag(); }

  /// Set an attribute with the given name to the given value.
  template< typename T >
  void setAttribute( const std::string& key, const T& value ) {
    appendAttribute( mXml, key, value );
  }
  /// Get a named attribute for the underlying XmlObject.
  const std::string& attribute( const std::string& key ) const {
    return mXml->attribute( key );
  }

  /// Append a line of text content to the XmlObject.
  void appendContent( const std::string& content ) {
    mXml->appendContent( content );
  }
  /// Get a line of content from the XmlObject.
  const std::string& contentLine( unsigned int line ) const {
    return mXml->contentLine( line );
  }
};

XDM_NAMESPACE_END

#endif // xdm_XmlMetadataWrapper_hpp

