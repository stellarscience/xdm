#include <xdmGrid/Domain.hpp>

XDM_GRID_NAMESPACE_BEGIN

void Domain::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::CompositeDataItem::writeMetadata( xml );
  xml.setTag( "Domain" );
}

XDM_GRID_NAMESPACE_END

