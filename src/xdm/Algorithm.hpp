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
#ifndef xdm_Algorithm_hpp
#define xdm_Algorithm_hpp

#include <algorithm>
#include <string>
#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Binary predicate that returns true if two dereferenced values are equal.
template< typename T >
class EqualPointerValue {
public:
  bool operator()( const T& lhs, const T& rhs ) {
    return ( *lhs == *rhs );
  }
};

/// Determine if two ordered collections are equal. Ordered collections are
/// considered equal if they have the same elements in the same order and they
/// contain the same number of elements.
template< typename InputIterator1, typename InputIterator2,
typename BinaryPredicate >
bool orderedCollectionsEqual(
    InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2,
    BinaryPredicate comp ) {

  // Sizes must be equal.
  if ( std::distance( first1, last1 ) != std::distance( first2, last2 ) ) {
    return false;
  }

  // Since the sizes are equal and the collections must be equal in order,
  // return the value of std::equal.
  return std::equal( first1, last1, first2, comp );
}

/// Determine if two ordered collections are equal using the default
/// equality comparator.
template< typename InputIterator1, typename InputIterator2 >
bool orderedCollectionsEqual(
    InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2 ) {

  typedef typename std::iterator_traits< InputIterator1 >::value_type Value;
  return orderedCollectionsEqual( first1, last1, first2, last2,
                                  std::equal_to< Value >() );
}

/// Make a suitable ValueType for use in mutating STL algorithms.
template< typename T >
class MakeMutable {
public:
  typedef T ValueType;
};

/// Determine the mutable type for a const qualified type.
template< typename T >
class MakeMutable< const T > {
public:
  typedef typename MakeMutable< T >::ValueType ValueType;
};

/// Make a standard pair mutable.
template< typename T, typename U >
class MakeMutable< std::pair< T, U > > {
public:
  typedef std::pair<
      typename MakeMutable< T >::ValueType,
      typename MakeMutable< U >::ValueType > ValueType;
};

/// Determine if two unordered collections are equal. Unordered collections are
/// considered equal if they have the same elements in any order and they
/// contain the same number of elements.
template< typename InputIterator1, typename InputIterator2,
typename BinaryPredicate >
bool unorderedCollectionsEqual(
    InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2,
    BinaryPredicate comp ) {

  // Sizes must be equal.
  ptrdiff_t size1 = std::distance( first1, last1 );
  ptrdiff_t size2 = std::distance( first2, last2 );
  if ( size1 != size2 ) {
    return false;
  }

  // Since order doesn't matter, the ranges need to be sorted for comparison.
  typedef typename MakeMutable<
      typename std::iterator_traits< InputIterator1 >::value_type >::ValueType
      ValueType1;
  typedef typename MakeMutable<
      typename std::iterator_traits< InputIterator2 >::value_type >::ValueType
      ValueType2;
  std::vector< ValueType1 > sortedRange1( size1 );
  std::vector< ValueType2 > sortedRange2( size2 );
  std::copy( first1, last1, sortedRange1.begin() );
  std::copy( first2, last2, sortedRange2.begin() );
  std::sort( sortedRange1.begin(), sortedRange1.end() );
  std::sort( sortedRange2.begin(), sortedRange2.end() );
  return std::equal(
      sortedRange1.begin(),
      sortedRange1.end(),
      sortedRange2.begin(),
      comp );
}

/// Determine if two unordered collections are equal using the default
/// equality comparator.
template< typename InputIterator1, typename InputIterator2 >
bool unorderedCollectionsEqual(
    InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2 ) {

  typedef typename std::iterator_traits< InputIterator1 >::value_type Value;
  return unorderedCollectionsEqual( first1, last1, first2, last2,
                                    std::equal_to< Value >() );
}

/// Trim leading and trailing white space from a string.
inline void trim( std::string& io ) {
  std::string::size_type pos = io.find_last_not_of( " \t" );
  if ( pos != std::string::npos ) {
    io.erase( pos + 1 );
    pos = io.find_first_not_of( " \t" );
    if ( pos != std::string::npos ) io.erase( 0, pos );
  }
}

/// Convert a string to upper case.
inline void toUpper( std::string& s ) {
  for ( std::string::iterator c = s.begin(); c != s.end(); ++c ) {
    if ( 'a' <= *c && *c <= 'z' ) *c -= ('a' - 'A');
  }
}

XDM_NAMESPACE_END

#endif // xdm_Algorithm_hpp
