#ifndef xdm_DataShape_hpp
#define xdm_DataShape_hpp

#include <algorithm>
#include <cassert>
#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

template< typename T = std::size_t >
class DataShape {
private:
  T mRank;
  typedef std::vector< T > DimensionList;
  DimensionList mDimensions;

public:

  typedef T size_type;

  DataShape() :
    mRank( 0 ),
    mDimensions() {
  }
  explicit DataShape( size_type rank ) :
    mRank( rank ),
    mDimensions( rank ) {
  }
 
  template< typename U > friend class DataShape;
  template< typename U >
  DataShape( const DataShape< U >& other ) :
    mRank( other.mRank ),
    mDimensions( other.mRank ) {
    std::copy( 
      other.mDimensions.begin(), 
      other.mDimensions.end(),
      mDimensions.begin() );
  }

  ~DataShape() {
  }

  void setRank( size_type rank ) {
    mRank = rank;
    mDimensions.resize( rank );
  }

  size_type rank() const {
    return mRank;
  }

  size_type& operator[]( size_type i ) {
    assert( i < mRank );
    return mDimensions[i];
  }

  const size_type& operator[]( size_type i ) const {
    assert( i < mRank );
    return mDimensions[i];
  }
};

XDM_NAMESPACE_END

#endif // xdm_DataShape_hpp

