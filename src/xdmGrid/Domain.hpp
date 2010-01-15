#ifndef xdmGrid_Domain_hpp
#define xdmGrid_Domain_hpp

#include <xdm/CompositeDataItem.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Domain : public xdm::CompositeDataItem {
public:

  XDM_META_ITEM( Domain );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Domain_hpp

