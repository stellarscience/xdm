#include <xdmGrid/Attribute.hpp>

XDM_GRID_NAMESPACE_BEGIN

Attribute::Attribute( Type t, Center c ) :
  mType( t ),
  mCenter( c ) {
}

Attribute::~Attribute() {
}

XDM_GRID_NAMESPACE_END

