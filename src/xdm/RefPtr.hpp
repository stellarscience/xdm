//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_RefPtr_hpp
#define xdm_RefPtr_hpp

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Smart pointer to manage the memory occupied by ReferencedObject subclasses.
/// Much of this implementation is inspired by the OpenSceneGraph library.
template< typename T >
class RefPtr {
public:
  typedef T value_type;

  RefPtr() : mPtr( 0 ) {}
  RefPtr( T* ptr ) : mPtr( ptr ) { 
    if ( mPtr ) {
      mPtr->addReference();
    }
  }
  RefPtr( const RefPtr& other ) : mPtr( other.mPtr ) {
    if ( mPtr ) {
      mPtr->addReference();
    }
  }

  ~RefPtr() {
    if ( mPtr ) {
      mPtr->removeReference();
    }
    mPtr = 0;
  }

  RefPtr& operator=( const RefPtr& rhs ) {
    assign( rhs.mPtr );
    return *this;
  }

  template< typename U >
  RefPtr& operator=( const RefPtr< U >& rhs ) {
    assign( rhs.mPtr );
    return *this;
  }

  template< typename U >
  RefPtr& operator=( U* rhs ) {
    assign( rhs );
    return *this;
  }

  void swap( RefPtr& other ) {
    T* tmp = mPtr;
    mPtr = other.mPtr;
    other.mPtr = tmp;
  }

  operator T*() const { return mPtr; }
  T& operator*() const { return *mPtr; }
  T* operator->() const { return mPtr; }
  T* get() const { return mPtr; }

  bool operator!() const { return mPtr == 0; }
  bool valid() const { return mPtr != 0; }

  T* release() {
    T* tmp = mPtr;
    if ( mPtr ) {
      mPtr->removeReferenceWithoutDelete();
    }
    mPtr = 0;
    return tmp;
  }

private:
  T* mPtr;
  template< typename U > friend class RefPtr;
  template< typename U > void assign( U* u ) {
    if ( mPtr == u ) return;
    T* tmp = mPtr;
    mPtr = u;
    // the order below is important:  if u is referenced by the original mPtr
    // then removing the original reference before adding a reference to u could
    // cause u to be deleted.
    if ( mPtr ) mPtr->addReference();
    if ( tmp ) tmp->removeReference();
  }
};

XDM_NAMESPACE_END

#endif // xdm_RefPtr_hpp

