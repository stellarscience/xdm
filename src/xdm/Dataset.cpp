#include <xdm/Dataset.hpp>

XDM_NAMESPACE_BEGIN

Dataset::Dataset() {
}

Dataset::~Dataset() {
}

void Dataset::initialize( const DataShape<>& shape, std::iostream& content ) {
  initializeImplementation( shape, content );
}

void Dataset::serialize(
  const Array& data,
  const SlabMap<>& slabMap,
  std::iostream& content )
{
  serializeImplementation( data, slabMap, content );
}

void Dataset::finalize() {
  finalizeImplementation();
}

XDM_NAMESPACE_END

