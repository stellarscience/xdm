#ifndef xdm_DataShape_hpp
#define xdm_DataShape_hpp

#include <algorithm>
#include <cassert>
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

private:
  size_type mRank;
  Index mDimensions;

public:


  /// Default constructor initializes to empty shape (rank 0).
  DataShape() :
    mRank( 0 ),
    mDimensions() {
  }

  /// Constructor initializes from a rank.  Allocates the array of dimensions
  /// corresponding to the rank.
  explicit DataShape( size_type rank ) :
    mRank( rank ),
    mDimensions( rank ) {
  }
 
  /// Provide privelaged access for DataShape classes with a different
  /// fundamental size representation.
  template< typename U > friend class DataShape;

  /// Initialize from a DataShape with a different size representation.
  template< typename U >
  DataShape( const DataShape< U >& other ) :
    mRank( other.mRank ),
    mDimensions( other.mRank ) {
    std::copy( 
      other.mDimensions.begin(), 
      other.mDimensions.end(),
      mDimensions.begin() );
  }

  /// Destructor.
  ~DataShape() {
  }

  void setRank( size_type rank ) {
    mRank = rank;
    mDimensions.resize( rank );
  }

  size_type rank() const {
    return mRank;
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

  /// Get the dimension of the data at the specified index.
  size_type& operator[]( size_type i ) {
    assert( i < mRank );
    return mDimensions[i];
  }

  /// Get the const dimension of the data at the specified index.
  const size_type& operator[]( size_type i ) const {
    assert( i < mRank );
    return mDimensions[i];
  }
};

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

/// Print a DataShape to an ostream.
template< typename T >
std::ostream& operator<<( std::ostream& ostr, const DataShape< T >& shape ) {
  typedef DataShape< T > TypedDataShape;
  typename TypedDataShape::size_type rank = shape.rank();
  if ( rank == 0 ) {
    return ostr;
  }
  typename TypedDataShape::ConstDimensionIterator begin = shape.begin();
  ostr << *begin++;
  typename TypedDataShape::ConstDimensionIterator current;
  for ( current = begin; current != shape.end(); ++current ) {
    ostr << " " << *current;
  }
  return ostr;
}

XDM_NAMESPACE_END

#endif // xdm_DataShape_hpp

