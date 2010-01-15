#ifndef xdmHdf_FileIdentifierRegistry_hpp
#define xdmHdf_FileIdentifierRegistry_hpp

#include <xdmHdf/FileIdentifier.hpp>

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <map>
#include <string>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

/// Singleton registry to hold references to an HDF file identifier.  According
/// to the HDF documentation, a single application should open a file only once.
/// This registry allows that to happen by caching the identifier for all open
/// files indexed by filename.
class FileIdentifierRegistry : public xdm::ReferencedObject {
public:
  static FileIdentifierRegistry* instance();
  
  /// Get or create an identifier for a given file name.
  xdm::RefPtr< FileIdentifier > findOrCreateIdentifier( 
    const std::string& key );

private:
  FileIdentifierRegistry();

  static xdm::RefPtr< FileIdentifierRegistry > sInstance;
  typedef std::map< std::string, xdm::RefPtr< FileIdentifier > >
    IdentifierMapping;
  IdentifierMapping mIdentifierMapping;
};

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_FileIdentifierRegistry_hpp

