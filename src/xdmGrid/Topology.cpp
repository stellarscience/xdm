#include <xdmGrid/Topology.hpp>

XDM_GRID_NAMESPACE_BEGIN

void Topology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::CompositeDataItem::writeMetadata( xml );
  xml.setTag( "Toplogy" );
}

XDM_GRID_NAMESPACE_END
