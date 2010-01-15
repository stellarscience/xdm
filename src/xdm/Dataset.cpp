#include <xdm/Dataset.hpp>

XDM_NAMESPACE_BEGIN

Dataset::Dataset() :
  mUpdateCallback() {
}

Dataset::~Dataset() {
}

BasicDatasetUpdateCallback* Dataset::updateCallback() {
  return mUpdateCallback;
}

const BasicDatasetUpdateCallback* Dataset::updateCallback() const {
  return mUpdateCallback;
}

void Dataset::setUpdateCallback( BasicDatasetUpdateCallback* callback ) {
  mUpdateCallback = callback;
}
  

void Dataset::update() {
  if ( mUpdateCallback.valid() ) {
    mUpdateCallback->update( this );
  }
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

