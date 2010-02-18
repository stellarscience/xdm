//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
//
// This file is part of XDM
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the Impied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------------------------------------------------------
#ifndef xdm_VectorRef_hpp
#define xdm_VectorRef_hpp

#include <xdm/RefPtr.hpp>
#include <xdm/ReferencedObject.hpp>

#include <vector>

#include <cassert>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

template< typename T > class VectorRef;
template< typename T > class ConstVectorRef;
template< typename T > class VectorRefImp;
template< typename T > class VectorBase;

/// A VectorRef is a thin object that refers to a set of data and provides vector-like
/// data access semantics with operator[]. It provides a vector-like "view" of a set of
/// data that may or may not be contiguous in memory. This class was designed so that vector
/// operations could be performed on e.g. nodal xyz coordinates without resorting to dimension-
/// by-dimension access in the case where x, y, and z are stored in separate arrays. The class
/// provides a common interface for that case as well as arrays where node coordinates are
/// stored as xyzxyzxyzxyz, and so on.
///
/// Currently, VectorRef ALWAYS refers to persistent data, and there are no Impementations
/// of VectorRefImp that are self-contained. The copy constructor preserves the reference
/// semantics. Thus, if a self-contained vector is necessary, the user should copy the data
///  out of the VectorRef into a more suitable vector.
template< typename T >
class VectorRef : public VectorBase< T > {
public:
  VectorRef( xdm::RefPtr< VectorRefImp< T > > imp, std::size_t index ) :
    VectorBase< T >( imp, index ) {}

  VectorRef( const VectorRef< T >& other ) :
    VectorBase< T >( other ) {}

  VectorRef< T >& operator=( const VectorRef< T >& other ) {
    assert( this->size() == other.size() );
    for ( std::size_t i = 0; i < this->size(); ++i ) {
      (*this)[i] = other[i];
    }
    return *this;
  }

  VectorRef< T >& operator=( const ConstVectorRef< T >& other ) {
    assert( this->size() == other.size() );
    for ( std::size_t i = 0; i < this->size(); ++i ) {
      (*this)[i] = other[i];
    }
    return *this;
  }

  T& operator[]( std::size_t i ) {
    return const_cast< T& >(
      static_cast< const VectorBase< T >& >( *this )[i] );
  }

  using VectorBase< T >::operator[];

private:
  // Copy construction from a ConstVectorRef is not possible because it would
  // allow non-const reference of the same data.
  VectorRef( const ConstVectorRef< T >& other );
};

/// A const version of VectorRef. Non-const access of the data is not allowed.
template< typename T >
class ConstVectorRef : public VectorBase< T > {
public:
  ConstVectorRef( xdm::RefPtr< VectorRefImp< T > > imp, std::size_t index ) :
    VectorBase< T >( imp, index ) {}

  ConstVectorRef( const VectorRef< T >& other ) :
    VectorBase< T >( other ) {}

  ConstVectorRef( const ConstVectorRef< T >& other ) :
    VectorBase< T >( other ) {}

  /// This is intended to be used like a const_cast.
  VectorRef< T > removeConstness() const {
    VectorRef< T > ret( xdm::RefPtr< VectorRefImp< T > >(), 0 );
    copyReference( *this, ret );
    return ret;
  }

private:
  // Assignment of a ConstVectorRef is not allowed.
  ConstVectorRef< T >& operator=( const ConstVectorRef< T >& other );
  ConstVectorRef< T >& operator=( const VectorRef< T >& other );
};

/// VectorBase is the base class for VectorRef and ConstVectorRef. It only allows const access.
template< typename T >
class VectorBase {
public:
  /// @param imp The Impementation instance that will be used for this vector.
  /// @param index The index of this particular vector in the underlying collection of vectors.
  VectorBase( RefPtr< VectorRefImp< T > > imp, std::size_t index ) :
    mImp( imp ), mIndex( index ) {}

  const T& operator[]( std::size_t i ) const {
    return mImp->at( mIndex, i );
  }

  /// @returns The number of elements in the vector.
  std::size_t size() const {
    return mImp->size();
  }

protected:
  /// Derived class can construct from a base class reference.
  VectorBase( const VectorBase< T >& other ) :
    mImp( other.mImp ), mIndex( other.mIndex ) {}

  static void copyReference( const VectorBase< T >& source, VectorBase< T >& dest ) {
    dest.mImp = source.mImp;
    dest.mIndex = source.mIndex;
  }

private:
  RefPtr< VectorRefImp< T > > mImp;
  std::size_t mIndex;
};

/// The base Impementation class held by VectorRefs.
template< typename T >
class VectorRefImp : public ReferencedObject {
public:
  /// @param baseIndex The index of the vector in the underlying container of vectors.
  /// @param i The index of an element of the vector at @arg baseIndex, e.g. for an xyz vector,
  ///        i == 1 refers to the y value.
  virtual const T& at( std::size_t baseIndex, std::size_t i ) const = 0;

  /// @returns The number of elements in this vector.
  virtual std::size_t size() const = 0;
};

/// The Impementation for the trivial case where data is stored in a single array with the xyz
/// index cycling faster than the vector index.
/// Intrinsic state: single contiguous array of data.
/// Extrinsic state: index of specific location.
/// Interlaced arrays xyzxyzxyzxyz...
template< typename T >
class SingleArrayOfVectorsImp : public VectorRefImp< T > {
public:
  SingleArrayOfVectorsImp( T* xyzArray, std::size_t elementsPerVector );

  /// Assumes the array is laid out contiguously in dimension order.
  virtual const T& at( std::size_t baseIndex, std::size_t i ) const;

  virtual std::size_t size() const;

private:
  T* mData;
  std::size_t mSize;
};

/// The Impementation for the case where data is stored in multiple arrays where the vector index
/// cycles faster than the xyz index.
/// One array for each coordinate value of the nodes.
/// x1x2x3x4.... y1y2y3y4.... z1z2z3z4....
template< typename T >
class MultipleArraysOfVectorElementsImp : public VectorRefImp< T > {
public:
  MultipleArraysOfVectorElementsImp( const std::vector< T* >& arrays );

  virtual const T& at( std::size_t baseIndex, std::size_t i ) const;

  virtual std::size_t size() const;

private:
  std::vector< T* > mArrays;
  std::size_t mSize;
};

/// Tensor product Impementation for structured mesh. This references vectors whose elements are
/// embedded in multiple arrays, like MultipleArraysOfVectorElementsImp, but with the distinction
/// that the underlying data is just a tensor product (it is sparse). The at() function allows
/// apparently dense access (e.g. by node index), but under the hood, the data is stored only on
/// coordinate axes.
template< typename T >
class TensorProductArraysImp : public VectorRefImp< T > {
public:
  TensorProductArraysImp(
    const std::vector< T* >& coordinateAxisValues,
    const std::vector< std::size_t >& axisSizes );

  virtual const T& at( std::size_t baseIndex, std::size_t i ) const;

  virtual std::size_t size() const;

private:
  std::vector< T* > mCoordinateAxisValues;
  std::vector< std::size_t > mAxisSizes;
  std::size_t mSize;
};

//----------------------- Impementations --------------------------------------
template< typename T >
SingleArrayOfVectorsImp< T >::SingleArrayOfVectorsImp( T* xyzArray, std::size_t elementsPerVector ) :
  mData( xyzArray ), mSize( elementsPerVector ) {
}

template< typename T >
const T& SingleArrayOfVectorsImp< T >::at( std::size_t baseIndex, std::size_t i ) const {
  return mData[ mSize * baseIndex + i ];
}

template< typename T >
std::size_t SingleArrayOfVectorsImp< T >::size() const {
  return mSize;
}

template< typename T >
MultipleArraysOfVectorElementsImp< T >::MultipleArraysOfVectorElementsImp(
  const std::vector< T* >& arrays ) :
    mArrays( arrays ), mSize( arrays.size() ) {
}

template< typename T >
const T& MultipleArraysOfVectorElementsImp< T >::at( std::size_t baseIndex, std::size_t i ) const {
  return mArrays[i][baseIndex];
}

template< typename T >
std::size_t MultipleArraysOfVectorElementsImp< T >::size() const {
  return mSize;
}

template< typename T >
TensorProductArraysImp< T >::TensorProductArraysImp(
  const std::vector< T* >& coordinateAxisValues,
  const std::vector< std::size_t >& axisSizes ) :
    mCoordinateAxisValues( coordinateAxisValues ),
    mAxisSizes( axisSizes ),
    mSize( axisSizes.size() ) {
  assert( axisSizes.size() == coordinateAxisValues.size() );
}

template< typename T >
const T& TensorProductArraysImp< T >::at( std::size_t baseIndex, std::size_t i ) const {
  // choose a convention for indexing multi-dimension structured data and
  // stick with it. Following XDMF, let's say z is always considered to be
  // the slowest varying dimension, y next, x fastest.
  std::size_t blockSize = 1;
  std::size_t location[ mSize ];
  for ( int dimension = 0; dimension < mSize; ++dimension ) {
    location[ dimension ] = baseIndex / blockSize % mAxisSizes[ dimension ];
    blockSize *= mAxisSizes[ dimension ];
  }
  return mCoordinateAxisValues[i][location[i]];
}

template< typename T >
std::size_t TensorProductArraysImp< T >::size() const {
  return mSize;
}

XDM_NAMESPACE_END

#endif // xdm_VectorRef_hpp
