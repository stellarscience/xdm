#include <xdm/Dataset.hpp>

XDM_NAMESPACE_BEGIN

Dataset::Dataset() :
  mInitializeCallback( NULL ),
  mSerializeCallback( NULL ),
  mFinalizeCallback( NULL ) {
}

Dataset::~Dataset() {
}

void Dataset::initialize(
  primitiveType::Value type,
  const DataShape<>& shape, 
  std::iostream& content ) {
  if ( mInitializeCallback ) {
    mInitializeCallback->initializeImplementation( this, type, shape, content );
  } else {
    initializeImplementation( type, shape, content );
  }
}

void Dataset::serialize(
  const StructuredArray& data,
  const HyperSlabMap<>& slabMap,
  std::iostream& content )
{
  if ( mSerializeCallback ) {
    mSerializeCallback->serializeImplementation( this, data, slabMap, content );
  } else {
    serializeImplementation( data, slabMap, content );
  }
}

void Dataset::finalize() {
  if ( mFinalizeCallback ) {
    mFinalizeCallback->finalizeImplementation( this );
  } else {
    finalizeImplementation();
  }
}

void Dataset::setInitializeCallback( InitializeCallback* icb ) {
  mInitializeCallback = icb;
}

void Dataset::setSerializeCallback( SerializeCallback* scb ) {
  mSerializeCallback = scb;
}

void Dataset::setFinalizeCallback( FinalizeCallback* fcb ) {
  mFinalizeCallback = fcb;
}

XDM_NAMESPACE_END

