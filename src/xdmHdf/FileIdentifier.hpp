#ifndef xdmHdf_FileIdentifier_hpp
#define xdmHdf_FileIdentifier_hpp

#include <xdmHdf/ResourceIdentifier.hpp>

#include <hdf5.h>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

class FileIdentifierReleaseFunctor {
public:
  herr_t operator()( hid_t identifier ) {
    return H5Fclose( identifier );
  }
};

typedef ResourceIdentifier< FileIdentifierReleaseFunctor > FileIdentifier;

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_FileIdentifier_hpp

