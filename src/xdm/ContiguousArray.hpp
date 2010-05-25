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
#ifndef xdm_ContiguousArray_hpp
#define xdm_ContiguousArray_hpp

#include <xdm/TypedStructuredArray.hpp>



namespace xdm {

/// Wrapper class to provide access to a contiguous, dynamically allocated array
/// as a TypedStructuredArray. This class does not take ownership of the array.
/// This allows clients to pass their existing memory datastructures to the xdm
/// interfaces that expect TypedStructuredArray for typed operations or
/// StructuredArray access for untyped operations.
template< typename T >
class ContiguousArray : public TypedStructuredArray< T > {
  typedef TypedStructuredArray< T > Base;
public:

  typedef typename Base::value_type value_type;
  typedef typename Base::pointer pointer;
  typedef typename Base::const_pointer const_pointer;
  typedef typename Base::iterator iterator;
  typedef typename Base::const_iterator const_iterator;
  typedef typename Base::reference reference;
  typedef typename Base::const_reference const_reference;
  typedef typename Base::size_type size_type;

  /// Constructor initializes the correct values in TypedStructuredArray.
  ContiguousArray( T* data, size_t size ) :
    TypedStructuredArray< T >( data, size ) {}

  /// Destructor does not free the underlying memory.
  virtual ~ContiguousArray() {}

  /// Resizing moves the memory.
  virtual void resize( size_t count ) {
    if ( count < Base::protectedSize() ) {
      // don't allocate, just reset the size.
      TypedStructuredArray< T >::setSize( count );
    } else {
      // try to reallocate the memory
      T* tmp;
      try {
        tmp = new T[ count ];
      } catch ( std::bad_alloc ) {
        throw NotEnoughMemoryError( count * sizeof( T ) );
      }

      // try to copy the old memory over.
      try {
        // copy the the existing elements into place.
        std::uninitialized_copy(
          Base::typedData(),
          Base::typedData() + Base::protectedSize(),
          tmp );
        // fill the rest with default elements.
        std::uninitialized_fill( tmp + Base::protectedSize(), tmp + count, T() );
      } catch ( ... ) {
        delete [] tmp;
        throw;
      }
      // delete the old data and update internal state.
      delete [] Base::typedData();
      Base::setData( tmp );
      Base::setSize( count );
    }
  }

  /// Provide access to the protected setSize() function from
  /// TypedStructuredArray.
  using Base::setSize;
};

/// Convenience template to construct a StructuredArray with the right type
/// arguments. This function uses template deduction to determine the right type
/// of TypedStructuredArray to instantiate.
template< typename T >
RefPtr< StructuredArray >
createStructuredArray( T* data, size_t size ) {
  return makeRefPtr( new ContiguousArray< T >( data, size ) );
}

} // namespace xdm

#endif // xdm_ContiguousArray_hpp
