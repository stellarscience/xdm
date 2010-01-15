//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/ReferencedObject.hpp>

namespace {
  void deleteReferencedObject( xdm::ReferencedObject* object ) {
    delete object;
  }
} // namespace anon

XDM_NAMESPACE_BEGIN

ReferencedObject::ReferencedObject() :
  mReferenceCount( 0 ) {
}

ReferencedObject::~ReferencedObject() {
}

void ReferencedObject::addReference() {
  mReferenceCount++;
}

void ReferencedObject::removeReference() {
  mReferenceCount--;
  if ( mReferenceCount <= 0 ) {
    deleteReferencedObject( this );
  }
}

void ReferencedObject::removeReferenceWithoutDelete() {
  mReferenceCount--;
}

int ReferencedObject::referenceCount() const {
  return mReferenceCount;
}

XDM_NAMESPACE_END

