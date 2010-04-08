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
#ifndef xdm_DataShape_hpp
#define xdm_DataShape_hpp

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <ostream>
#include <string>
#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Provides information regarding rank and dimensions for data.  This class is
/// templated on the size type of the rank and dimensions of the data to provide
/// interoperability with libraries that may use a different fundamental type to
/// represent size.  The size type defaults to std::size_t
template< typename T = std::size_t >
class DataShape {
public:
  typedef T size_type;
  typedef std::vector< T > Index;
  typedef typename std::vector< T >::iterator DimensionIterator;
  typedef typename std::vector< T >::const_iterator ConstDimensionIterator;
  typedef typename std::vector< T >::reverse_iterator ReverseDimensionIterator;
  typedef typename std::vector< T >::const_reverse_iterator ConstReverseDimensionIterator;

private:
  Index mDimensions;

public:

  /// Default constructor initializes to empty shape (rank 0).
  DataShape() :
    mDimensions() {
  }

  /// Constructor initializes from a rank.  Allocates the array of dimensions
  /// corresponding to the rank.
  explicit DataShape( size_type rank ) :
    mDimensions( rank ) {
  }
 
  /// Provide privelaged access for DataShape classes with a different
  /// fundamental size representation.
  template< typename U > friend class DataShape;

  /// Initialize from a DataShape with a different size representation.
  template< typename U >
  DataShape( const DataShape< U >& other ) :
    mDimensions( other.mDimensions.size() ) {
    std::copy( 
      other.mDimensions.begin(), 
      other.mDimensions.end(),
      mDimensions.begin() );
  }

  /// Destructor.
  ~DataShape() {
  }

  void setRank( size_type rank ) {
    mDimensions.resize( rank );
  }

  size_type rank() const {
    return mDimensions.size();
  }

  DimensionIterator begin() {
    return mDimensions.begin();
  }
  ConstDimensionIterator begin() const {
    return mDimensions.begin();
  }

  DimensionIterator end() {
    return mDimensions.end();
  }
  ConstDimensionIterator end() const {
    return mDimensions.end();
  }

  ReverseDimensionIterator rbegin() {
    return mDimensions.rbegin();
  }
  ConstReverseDimensionIterator rbegin() const {
    return mDimensions.rbegin();
  }

  ReverseDimensionIterator rend() {
    return mDimensions.rend();
  }
  ConstReverseDimensionIterator rend() const {
    return mDimensions.rend();
  }

  /// Get the dimension of the data at the specified index.
  size_type& operator[]( size_type i ) {
    assert( i < mDimensions.size() );
    return mDimensions[i];
  }

  /// Get the const dimension of the data at the specified index.
  const size_type& operator[]( size_type i ) const {
    assert( i < mDimensions.size() );
    return mDimensions[i];
  }

  /// Push a dimension onto the shape, increasing the rank by 1.
  void push_back( size_type i ) {
    mDimensions.push_back( i );
  }

  /// Reverse the dimension order in a data shape.  This will take the given
  /// DataShape and reverse the dimension order so that a shape with dimensions 
  /// [d(0) d(1) ...d(n)] becomes [d(n) d(n-1) ... d(1) d(0)].
  void reverseDimensionOrder() {
    std::reverse( begin(), end() );
  }
};

template< typename T >
bool operator==( const DataShape<T>& lhs, const DataShape<T>& rhs ) {
  if ( lhs.rank() != rhs.rank() ) {
    return false;
  }
  return std::equal( lhs.begin(), lhs.end(), rhs.begin() );
}

template< typename T >
bool operator!=( const DataShape< T >& lhs, const DataShape< T >& rhs ) {
  return !( lhs == rhs );
}

/// Reverse the dimension order in a data shape.  This will take the given
/// DataShape and reverse the dimension order so that a shape with dimensions 
/// [d(0) d(1) ... d(n-1) d(n)] becomes [d(n) d(n-1) ... d(1) d(0)].
template< typename T >
void reverseDimensionOrder( DataShape< T >& shape ) {
  shape.reverseDimensionOrder();
}

/// Find the linear index for a shape within another shape that represents
/// the maximum dimensions for the input shape. This follows the C (fastest
/// varying dimension first) ordering convention.
/// @param indexShape The index into the contextShape.
/// @param contextShape The maximum index in each dimension.
template< typename T >
typename DataShape< T >::size_type linearize(
  const DataShape< T >& indexShape,
  const DataShape< T >& contextShape ) {
  typedef typename DataShape< T >::ConstDimensionIterator Iterator;
  typedef typename DataShape< T >::size_type SizeType;

  assert( indexShape.rank() == contextShape.rank() );

  SizeType lowerProduct;
  SizeType result = 0;
  Iterator indexBegin = indexShape.begin();
  Iterator indexEnd = indexShape.end();
  Iterator contextBegin = contextShape.begin();
  Iterator contextEnd = contextShape.end();
  for ( Iterator dim = indexBegin; dim != indexEnd; ++dim ) {
    lowerProduct = std::accumulate(
      contextBegin + (dim - indexBegin) + 1, contextEnd,
      1, std::multiplies< T >() );
    result += (*dim) * lowerProduct;
  }
  return result;
}

/// Make a DataShape given a space separated string with the dimensions.
DataShape<> makeShape( const std::string& dimensions );

/// Make a DataShape of rank 1 with the given dimension.
DataShape<> makeShape( DataShape<>::size_type i );

/// Make a DataShape of rank 2 with the given dimensions.
DataShape<> makeShape( DataShape<>::size_type i, DataShape<>::size_type j );

/// Make a DataShape of rank 3 with the given dimensions.
DataShape<> makeShape( 
  DataShape<>::size_type i,
  DataShape<>::size_type j,
  DataShape<>::size_type k );

/// Make a DataShape of rank 4 with the given dimensions.
DataShape<> makeShape(
  DataShape<>::size_type i,
  DataShape<>::size_type j,
  DataShape<>::size_type k,
  DataShape<>::size_type l );

/// Make a new shape by contracting the input space by the specified amount in
/// each dimension.
DataShape<> makeContraction( const DataShape<>& space, DataShape<>::size_type n );

/// Print a DataShape to an ostream.
template< typename T >
std::ostream& operator<<( std::ostream& ostr, const DataShape< T >& shape ) {
  typedef DataShape< T > TypedDataShape;
  typedef typename TypedDataShape::size_type SizeType;
  typedef typename DataShape< T >::ConstDimensionIterator Iterator;

  if ( shape.rank() == 0 ) {
    return ostr;
  }

  Iterator current = shape.begin();
  ostr << *current++;
  while ( current != shape.end() ) {
    ostr << " " << *current++;
  }
  return ostr;
}

XDM_NAMESPACE_END

#endif // xdm_DataShape_hpp

