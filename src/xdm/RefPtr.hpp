//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.      
//                                                                             
// This file is part of XDM                                                    
//                                                                             
// This program is free software: you can redistribute it and/or modify it     
// under the terms of the GNU Lesser General Public License as published by    
// the Free Software Foundation, either version 3 of the License, or (at your  
// option) any later version.                                                  
//                                                                             
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        
// License for more details.                                                   
//                                                                             
// You should have received a copy of the GNU Lesser General Public License    
// along with this program.  If not, see <http://www.gnu.org/licenses/>.       
//                                                                             
//------------------------------------------------------------------------------
#ifndef xdm_RefPtr_hpp
#define xdm_RefPtr_hpp

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Smart pointer implementation to manage ReferencedObject types. This class
/// manages an intrusive reference count for objects it points to. It expects
/// the pointer it manages to have the methods addReference(),
/// removeReference(), and removeReferenceWithoutDelete(). It uses these methods
/// to manage a reference count for an object and determine when an object has
/// no more clients and can therefore be freed. The xdm::ReferencedObject class
/// has an implementation of these methods, so this class can manage subclasses
/// of ReferencedObject.
///
/// This implementation is from by the OpenSceneGraph library.
///
/// @param T The value type for the held pointer.
template< typename T >
class RefPtr {
public:
  typedef T value_type;

  /// Initialize with a NULL pointer.
  RefPtr() : mPtr( 0 ) {}

  /// Initialize given a raw pointer to manage.
  RefPtr( T* ptr ) : mPtr( ptr ) { 
    if ( mPtr ) {
      mPtr->addReference();
    }
  }

  /// Initialize from a reference counted pointer to the same type.
  RefPtr( const RefPtr& other ) : mPtr( other.mPtr ) {
    if ( mPtr ) {
      mPtr->addReference();
    }
  }

  /// Unreference the object, possibly deleting it.
  ~RefPtr() {
    reset();
  }

  /// Assign from a reference counted pointer to the same type.
  RefPtr& operator=( const RefPtr& rhs ) {
    assign( rhs.mPtr );
    return *this;
  }

  /// Assign from a reference counted pointer to a different type. The pointer
  /// type on the right hand side must be convertible to T.
  template< typename U >
  RefPtr& operator=( const RefPtr< U >& rhs ) {
    assign( rhs.mPtr );
    return *this;
  }

  /// Assign from a raw pointer to a different type. The pointer type on the
  /// right hand side must be convertible to T.
  template< typename U >
  RefPtr& operator=( U* rhs ) {
    assign( rhs );
    return *this;
  }

  /// Swap with another reference counted pointer to the same type.
  void swap( RefPtr& other ) {
    T* tmp = mPtr;
    mPtr = other.mPtr;
    other.mPtr = tmp;
  }

  /// Conversion to raw pointer.
  operator T*() const { return mPtr; }
  /// Pointer dereference.
  T& operator*() const { return *mPtr; }
  /// Member access.
  T* operator->() const { return mPtr; }
  /// Get access to the raw pointer.
  T* get() const { return mPtr; }

  /// Use the reference counter pointer in boolean expressions.
  bool operator!() const { return mPtr == 0; }
  /// Determine if a null pointer is held.
  bool valid() const { return mPtr != 0; }

  /// Release the pointer without deleting the object. To unreference the
  /// object, use reset() instead.  This is potentially unsafe: while it is
  /// guaranteed that the object won't be deleted by this call, if any other
  /// RefPtr's hold a reference to the same object, it could be deleted at a
  /// later time.
  T* release() {
    T* tmp = mPtr;
    if ( mPtr ) {
      mPtr->removeReferenceWithoutDelete();
    }
    mPtr = 0;
    return tmp;
  }

  /// Reset the pointer value to 0, unreferencing and possibly freeing the held
  /// object.
  void reset() {
    if ( mPtr ) {
      mPtr->removeReference();
    }
    mPtr = 0;
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

