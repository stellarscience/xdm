#ifndef xdmHdf_DataspaceIdentifier_hpp
#define xdmHdf_DataspaceIdentifier_hpp

#include <xdmHdf/ResourceIdentifier.hpp>

#include <xdm/DataShape.hpp>
#include <xdm/RefPtr.hpp>

#include <hdf5.h>

#include <string>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

class DataspaceReleaseFunctor {
public:
  herr_t operator()( hid_t identifier ) {
    return H5Sclose( identifier );
  }
};

typedef ResourceIdentifier< DataspaceReleaseFunctor > DataspaceIdentifier;

/// Create an HDF5 dataspace identifier with the given shape.  The returned
/// handle will be resource managed by a xdm::RefPtr.
xdm::RefPtr< DataspaceIdentifier > createDataspaceIdentifier(
  const xdm::DataShape<>& shape );

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_DataspaceIdentifier_hpp

