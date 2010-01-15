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
  const DataShape<>& shape ) {
  if ( mInitializeCallback ) {
    mInitializeCallback->initializeImplementation( this, type, shape );
  } else {
    initializeImplementation( type, shape );
  }
}

void Dataset::serialize(
  const StructuredArray* data )
{
  if ( mSerializeCallback ) {
    mSerializeCallback->serializeImplementation( this, data );
  } else {
    serializeImplementation( data );
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

