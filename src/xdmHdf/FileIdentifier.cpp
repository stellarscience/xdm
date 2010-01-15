#include <xdmHdf/FileIdentifier.hpp>
#include <xdmHdf/FileIdentifierRegistry.hpp>

#include <hdf5.h>

XDM_HDF_NAMESPACE_BEGIN

xdm::RefPtr< FileIdentifier >
createFileIdentifier( const std::string& filename ) {
  return FileIdentifierRegistry::instance()->findOrCreateIdentifier( filename );
}

XDM_HDF_NAMESPACE_END

