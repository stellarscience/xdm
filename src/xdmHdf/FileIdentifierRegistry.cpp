#include <xdmHdf/FileIdentifierRegistry.hpp>

#include <hdf5.h>

XDM_HDF_NAMESPACE_BEGIN

xdm::RefPtr< FileIdentifierRegistry > FileIdentifierRegistry::sInstance;

FileIdentifierRegistry* FileIdentifierRegistry::instance() {
  if ( ! sInstance.valid() ) {
    sInstance = new FileIdentifierRegistry;
  }
  return sInstance;
}

FileIdentifierRegistry::FileIdentifierRegistry() :
  mIdentifierMapping() {
}

xdm::RefPtr< FileIdentifier > FileIdentifierRegistry::findOrCreateIdentifier(
  const std::string& key ) {
  // try to find an existing identifier in the map
  IdentifierMapping::iterator it = mIdentifierMapping.find( key );
  if ( it != mIdentifierMapping.end() ) {
    return it->second;
  }

  // no such file opened yet, we create it here
  hid_t fileId = H5Fcreate( 
    key.c_str(), 
    H5F_ACC_TRUNC,
    H5P_DEFAULT,
    H5P_DEFAULT );

  xdm::RefPtr< FileIdentifier > result( new FileIdentifier( fileId ) );
  mIdentifierMapping[key] = result;
  return result;
}

XDM_HDF_NAMESPACE_END

