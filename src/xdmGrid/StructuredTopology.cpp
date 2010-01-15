#include <xdmGrid/StructuredTopology.hpp>

XDM_GRID_NAMESPACE_BEGIN

StructuredTopology::StructuredTopology() :
  Topology(),
  mShape() {
}

StructuredTopology::~StructuredTopology() {
}

void StructuredTopology::setShape( const xdm::DataShape<>& shape ) {
  mShape = shape;
}

const xdm::DataShape<>& StructuredTopology::shape() const {
  return mShape;
}

XDM_GRID_NAMESPACE_END

