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
#ifndef xdm_TypedStructuredArray_hpp
#define xdm_TypedStructuredArray_hpp

#include <xdm/PrimitiveType.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/StructuredArray.hpp>

#include <memory>

#include <cassert>



namespace xdm {

/// This abstract class defines an extension to the StructuredArray interface
/// to access typed array data within a StructuredArray. This allows clients
/// with knowledge of the underlying array data type write access to the array
/// data itself in addition to the query interface provided by StructuredArray.
/// Subclasses are responsible for providing the actual array implementation.
///
/// In order to maintain performance, this class holds a pointer to the
/// underlying array. This means it is very important that clients implement
/// the proper constructor behavior in setting the array location and size.
/// In addition, upon a resize() request, inheritors are responsible for
/// updating the location and size of the internal array as necessary.
///
/// In addition to providing the StructuredArray interface for data, this class
/// also provides STL compliant random access iterators for accessing the data
/// as well as indexing operators for array access.
template< typename T >
class TypedStructuredArray : public StructuredArray {
public:

  typedef T value_type; ///< The type of value held in the array.
  typedef T* pointer; ///< The pointer type for the array.
  typedef const T* const_pointer; ///< The immutable pointer type for the array.
  typedef T* iterator; ///< Random access iterator
  typedef const T* const_iterator; ///< Random access const iterator
  typedef T& reference; ///< Reference type for array elements.
  typedef const T& const_reference; ///< Constant reference type for array elements.
  typedef size_t size_type;

  /// Destructor.
  virtual ~TypedStructuredArray() {}

  //-- StructuredArray Query Interface --//

  /// Returns the data type corresponding to the value_type for the array.
  virtual primitiveType::Value dataType() const {
    return PrimitiveTypeInfo< T >::kValue;
  }

  /// Returns the element size in bytes of the value_type.
  virtual size_t elementSize() const {
    return PrimitiveTypeInfo< T >::kSize;
  }

  /// Returns the number of elements in the array.
  virtual size_t size() const {
    return mSize;
  }

  /// Returns the untyped pointer to the beginning of the array.
  virtual const void* data() const {
    return mData;
  }

  //-- Typed Data Access Interface --//

  /// Get the typed pointer for the data.
  pointer typedData() { return mData; }
  /// Get the constant typed pointer for the data.
  const_pointer typedData() const { return mData; }

  /// Get an iterator pointing to the beginning of the data.
  iterator begin() {
    return mData;
  }

  /// Get a const iterator pointing to the beginning of the data.
  const_iterator begin() const {
    return mData;
  }

  /// Get an iterator pointing to the end of the data.
  iterator end() {
    return mData + mSize;
  }

  /// Get a const iterator pointing to the end of the data.
  const_iterator end() const {
    return mData + mSize;
  }

  /// Index the ith element of the array.
  value_type& operator[]( size_t i ) {
    assert( i < mSize );
    return mData[i];
  }

  /// Index the const ith element of the array.
  const value_type& operator[] ( size_t i ) const {
    assert( i < mSize );
    return mData[i];
  }

protected:
  /// Construct the array with a NULL pointer and size 0.
  TypedStructuredArray() :
    mData( 0 ),
    mSize( 0 ) {
  }

  /// Construct the array with a pointer to its data and size.
  TypedStructuredArray( T* data, size_t size ) :
    StructuredArray(),
    mData( data ),
    mSize( size ) {
  }

  /// Set the pointer to the array. Inheritors must call this when the memory
  /// for the array is relocated.
  void setData( T* data ) {
    mData = data;
  }

  /// Set the number of elements in the array.  Inheritors must call this when
  /// the array size changes.
  void setSize( size_t size ) {
    mSize = size;
  }

  /// Get the size of the array. This is in addition to the public size() method
  /// because this one is protected and does not need to be virtual.
  size_t protectedSize() const {
    return mSize;
  }

private:
  T* mData;
  size_t mSize;
};

} // namespace xdm

#endif // xdm_TypedStructuredArray_hpp
