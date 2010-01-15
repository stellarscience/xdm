#ifndef xdmHdf_GroupIdentifier_hpp
#define xdmHdf_GroupIdentifier_hpp

#include <xdmHdf/ResourceIdentifier.hpp>

#include <xdm/RefPtr.hpp>

#include <hdf5.h>

#include <string>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

class GroupReleaseFunctor {
public:
  herr_t operator()( hid_t identifier ) {
    return H5Gclose( identifier );
  }
};

typedef ResourceIdentifier< GroupReleaseFunctor > GroupIdentifier;

xdm::RefPtr< GroupIdentifier > createGroupIdentifier( 
  hid_t parent, const std::string& name );

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_GroupIdentifier_hpp

