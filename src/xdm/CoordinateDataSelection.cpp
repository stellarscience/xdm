#include <xdm/CoordinateDataSelection.hpp>

#include <xdm/DataSelectionVisitor.hpp>

XDM_NAMESPACE_BEGIN

CoordinateDataSelection::CoordinateDataSelection() {
}

CoordinateDataSelection::CoordinateDataSelection( 
  const CoordinateArray<>& coordinates ) :
  mCoordinates( coordinates ) {
}

CoordinateDataSelection::~CoordinateDataSelection() {
}

const CoordinateArray<>& CoordinateDataSelection::coordinates() const {
  return mCoordinates;
}

void CoordinateDataSelection::setCoordinates( 
  const CoordinateArray<>& coordinates ) {
  mCoordinates = coordinates;
}

void CoordinateDataSelection::accept( DataSelectionVisitor& v ) const {
  v.apply( *this );
}

XDM_NAMESPACE_END

