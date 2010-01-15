#include <xdm/Dataset.hpp>

XDM_NAMESPACE_BEGIN

Dataset::Dataset() {
}

Dataset::~Dataset() {
}

void Dataset::initialize(
  primitiveType::Value type,
  const DataShape<>& shape ) {
  initializeImplementation( type, shape );
}

void Dataset::serialize(
  const StructuredArray* data,
  const DataSelectionMap& selectionMap )
{
  serializeImplementation( data, selectionMap );
}

void Dataset::finalize() {
  finalizeImplementation();
}

XDM_NAMESPACE_END

