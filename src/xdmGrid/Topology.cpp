#include <xdmGrid/Topology.hpp>

XDM_GRID_NAMESPACE_BEGIN

Topology::Topology() :
  xdm::CompositeDataItem() {
}

Topology::~Topology() {
}

void Topology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::CompositeDataItem::writeMetadata( xml );
  xml.setTag( "Topology" );
}

XDM_GRID_NAMESPACE_END
