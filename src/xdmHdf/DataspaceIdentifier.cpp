#include <xdmHdf/DataspaceIdentifier.hpp>

XDM_HDF_NAMESPACE_BEGIN

xdm::RefPtr< DataspaceIdentifier > createDataspaceIdentifier(
  const xdm::DataShape<>& shape ) {
  xdm::DataShape< hsize_t > hdfShape ( shape );
  hid_t identifier = H5Screate_simple( 
    hdfShape.rank(), 
    &hdfShape[0],
    NULL );
  return xdm::RefPtr< DataspaceIdentifier >( 
    new DataspaceIdentifier( identifier ) );
}

XDM_HDF_NAMESPACE_END

