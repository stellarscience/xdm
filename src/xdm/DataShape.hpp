#ifndef xdm_DataShape_hpp
#define xdm_DataShape_hpp

#include <algorithm>
#include <cassert>
#include <ostream>
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

/// Print a DataShape to an ostream.
template< typename T >
std::ostream& operator<<( std::ostream& ostr, const DataShape< T >& shape ) {
  typedef DataShape< T > TypedDataShape;
  ostr << shape.rank() << ": ";
  for ( typename TypedDataShape::size_type i = 0; i < shape.rank(); ++i ) {
    ostr << shape[i] << " ";
  }
  return ostr;
}

XDM_NAMESPACE_END

#endif // xdm_DataShape_hpp

