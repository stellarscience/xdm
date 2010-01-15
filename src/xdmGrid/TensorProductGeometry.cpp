#include <xdmGrid/TensorProductGeometry.hpp>

#include <cassert>

XDM_GRID_NAMESPACE_BEGIN

TensorProductGeometry::TensorProductGeometry( unsigned int dimension ) :
  Geometry( dimension ) {
  setNumberOfChildren( dimension );
}

TensorProductGeometry::~TensorProductGeometry() {
}

void TensorProductGeometry::setCoordinateValues( 
  unsigned int dim,
  xdm::RefPtr< xdm::DataItem > data ) {
  assert( dim < dimension() );
  setChild( dim, data );
}

XDM_GRID_NAMESPACE_END

