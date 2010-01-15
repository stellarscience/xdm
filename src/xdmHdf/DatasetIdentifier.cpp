#include <xdmHdf/DatasetIdentifier.hpp>

XDM_HDF_NAMESPACE_BEGIN

xdm::RefPtr< DatasetIdentifier > createDatasetIdentifier(
  hid_t parent,
  const std::string& name,
  int type,
  hid_t dataspace ) {
  // check if the dataset already exists within the given parent
  htri_t exists = H5Lexists( parent, name.c_str(), H5P_DEFAULT );
  if ( (exists >= 0) && exists ) {
    hid_t datasetId = H5Dopen( parent, name.c_str(), H5P_DEFAULT );
    // FIXME: make sure the type and dataspace requested match those on disk.
    return xdm::RefPtr< DatasetIdentifier >( 
      new DatasetIdentifier( datasetId ) );
  }

  // the dataset doesn't exist, we create it
  hid_t datasetId = H5Dcreate(
    parent,
    name.c_str(),
    type,
    dataspace,
    H5P_DEFAULT,
    H5P_DEFAULT,
    H5P_DEFAULT );
  return xdm::RefPtr< DatasetIdentifier >( new DatasetIdentifier( datasetId ) );
}

XDM_HDF_NAMESPACE_END

