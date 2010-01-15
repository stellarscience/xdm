#include <xdmGrid/Geometry.hpp>

XDM_GRID_NAMESPACE_BEGIN

Geometry::Geometry( unsigned int dimension ) :
  mDimension( dimension ) {
}

Geometry::~Geometry() {
}

void Geometry::setDimension( unsigned int dimension ) {
  mDimension = dimension;
}

unsigned int Geometry::dimension() const {
  return mDimension;
}

void Geometry::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::CompositeDataItem::writeMetadata( xml );
  xml.setTag( "Geometry" );
}

XDM_GRID_NAMESPACE_END

