#ifndef xdmHdf_DatasetIdentifier_hpp
#define xdmHdf_DatasetIdentifier_hpp

#include <xdmHdf/ResourceIdentifier.hpp>

#include <xdm/RefPtr.hpp>

#include <hdf5.h>

#include <string>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

class DatasetReleaseFunctor {
public:
  herr_t operator()( hid_t identifier ) {
    return H5Dclose( identifier );
  }
};

typedef ResourceIdentifier< DatasetReleaseFunctor > DatasetIdentifier;

xdm::RefPtr< DatasetIdentifier > createDatasetIdentifier(
  hid_t parent,
  const std::string& name,
  int type,
  hid_t dataspace );

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_DatasetIdentifier_hpp

