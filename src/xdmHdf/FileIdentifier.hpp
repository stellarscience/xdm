#ifndef xdmHdf_FileIdentifier_hpp
#define xdmHdf_FileIdentifier_hpp

#include <xdmHdf/ResourceIdentifier.hpp>

#include <xdm/RefPtr.hpp>

#include <hdf5.h>

#include <string>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

class FileIdentifierReleaseFunctor {
public:
  herr_t operator()( hid_t identifier ) {
    return H5Fclose( identifier );
  }
};

typedef ResourceIdentifier< FileIdentifierReleaseFunctor > FileIdentifier;

/// Non-member function to create a file identifier given a file name.
xdm::RefPtr< FileIdentifier > 
createFileIdentifier( const std::string& filename );

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_FileIdentifier_hpp

