#include <xdmGrid/Geometry.hpp>

#include <algorithm>

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

void Geometry::traverse( xdm::ItemVisitor& iv ) {
  std::for_each( begin(), end(), xdm::ApplyVisitor( iv ) );
}

void Geometry::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::Item::writeMetadata( xml );
  xml.setTag( "Geometry" );
}

XDM_GRID_NAMESPACE_END

