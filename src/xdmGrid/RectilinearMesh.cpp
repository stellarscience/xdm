#include <xdmGrid/RectilinearMesh.hpp>

#include <stdexcept>

#include <xdm/ThrowMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

RectilinearMesh::RectilinearMesh() :
  StructuredTopology() {
}

void RectilinearMesh::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  StructuredTopology::writeMetadata( xml );

  unsigned int rank = shape().rank();
  switch ( rank ) {
  case 2: 
    xml.setAttribute( "TopologyType", "2DRectMesh" );
    break;
  case 3:
    xml.setAttribute( "TopologyType", "3DRectMesh" );
    break;
  default:
    {
      std::stringstream msg;
      msg << "Unsupported number of dimensions for rectinlinear mesh: ";
      msg << rank;
      XDM_THROW( std::domain_error( msg.str() ) );
    }
    break;
  }
}


XDM_GRID_NAMESPACE_END

