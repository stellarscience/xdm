#include <xdmGrid/InterlacedGeometry.hpp>

#include <stdexcept>

#include <cassert>

#include <xdm/ThrowMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

InterlacedGeometry::InterlacedGeometry( unsigned int dimension ) :
  Geometry( dimension ) {
}

InterlacedGeometry::~InterlacedGeometry() {
}

void InterlacedGeometry::setCoordinateValues( xdm::DataItem* data ) {
  appendChild( data );
}

void InterlacedGeometry::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Geometry::writeMetadata( xml );

  unsigned int dim = dimension();
  switch ( dim ) {
  case 1:
    xml.setAttribute( "GeometryType", "X" );
    break;
  case 2:
    xml.setAttribute( "GeometryType", "XY" );
    break;
  case 3:
    xml.setAttribute( "GeometryType", "XYZ" );
    break;
  default:
    XDM_THROW( std::domain_error( "Unsupported number of dimensions" ) );
    break;
  }
}

XDM_GRID_NAMESPACE_END

