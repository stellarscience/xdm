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
#ifndef xdm_VectorStructuredArray_hpp
#define xdm_VectorStructuredArray_hpp

#include <xdm/TypedStructuredArray.hpp>

#include <stdexcept>
#include <vector>

#include <xdm/NamespaceMacro.hpp>
#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

/// StructuredArray that manages its own storage with a standard vector. The
/// lifetime of the data is tied to the lifetime of the StructuredArray.
template< typename T >
class VectorStructuredArray : public TypedStructuredArray< T > {
private:

  typedef TypedStructuredArray< T > Base;

  std::vector< T > mVector;

public:

  typedef typename Base::value_type value_type;
  typedef typename Base::pointer pointer;
  typedef typename Base::const_pointer const_pointer;
  typedef typename Base::iterator iterator;
  typedef typename Base::const_iterator const_iterator;
  typedef typename Base::reference reference;
  typedef typename Base::const_reference const_reference;
  typedef typename Base::size_type size_type;

  /// Default constructor initializes with empty storage.
  VectorStructuredArray() :
    TypedStructuredArray< T >(),
    mVector() {
  }

  /// Constructor takes a size and optional initialization value for all
  /// elements. If no initialization is specified, it is filled with a
  /// defaultly constructed value.
  VectorStructuredArray( size_t size, const_reference t = value_type() ) :
    TypedStructuredArray< T >(),
    mVector( size, t ) {
    TypedStructuredArray< T >::setData( &mVector[0] );
    TypedStructuredArray< T >::setSize( size );
  }

  virtual ~VectorStructuredArray() {}

  /// Resizes the underlying vector and updates the base class members.
  void resize( size_type n ) {
    mVector.resize( n );
    TypedStructuredArray< T >::setSize( n );
    TypedStructuredArray< T >::setData( &mVector[0] );
  }

  /// Reserves space for the vector without resizing.
  void reserve( size_type n) { mVector.reserve( n ); }

};

/// Create a vector structured array given a primitiveType::Value parameter.
inline RefPtr< StructuredArray >
makeVectorStructuredArray( primitiveType::Value type ) {
  switch ( type ) {
  case primitiveType::kChar:
    return makeRefPtr( new VectorStructuredArray< char > ); break;
  case primitiveType::kShort:
    return makeRefPtr( new VectorStructuredArray< short > ); break;
  case primitiveType::kInt:
    return makeRefPtr( new VectorStructuredArray< int > ); break;
  case primitiveType::kLongInt:
    return makeRefPtr( new VectorStructuredArray< long int > ); break;
  case primitiveType::kUnsignedChar:
    return makeRefPtr( new VectorStructuredArray< unsigned char > ); break;
  case primitiveType::kUnsignedShort:
    return makeRefPtr( new VectorStructuredArray< unsigned short > ); break;
  case primitiveType::kUnsignedInt:
    return makeRefPtr( new VectorStructuredArray< unsigned int > ); break;
  case primitiveType::kLongUnsignedInt:
    return makeRefPtr( new VectorStructuredArray< long unsigned int > ); break;
  case primitiveType::kFloat:
    return makeRefPtr( new VectorStructuredArray< float > ); break;
  case primitiveType::kDouble:
    return makeRefPtr( new VectorStructuredArray< double > ); break;
  default:
    XDM_THROW( std::runtime_error( "Unknown array type." ) );
  }
}

XDM_NAMESPACE_END

#endif // xdm_VectorStructuredArray_hpp

