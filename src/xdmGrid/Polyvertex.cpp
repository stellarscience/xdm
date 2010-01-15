#include <xdmGrid/Polyvertex.hpp>

XDM_GRID_NAMESPACE_BEGIN

Polyvertex::Polyvertex() :
  UnstructuredTopology() {
}

Polyvertex::~Polyvertex() {
}

void Polyvertex::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  UnstructuredTopology::writeMetadata( xml );

  xml.setAttribute( "TopologyType", "Polyvertex" );
}

XDM_GRID_NAMESPACE_END
