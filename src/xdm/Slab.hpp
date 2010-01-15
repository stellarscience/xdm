#ifndef xdm_Slab_hpp
#define xdm_Slab_hpp

#include <xdm/DataShape.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <cstdlib>

XDM_NAMESPACE_BEGIN

template< typename T = std::size_t >
class Slab {
private:
  typedef std::vector< T > IndexArray;
  DataShape<T> mDataShape;
  IndexArray mStart;
  IndexArray mStride;
  IndexArray mCount;

public:
  typedef T size_type;

  Slab() :
    mDataShape() {
  }
  Slab( const DataShape<T>& shape ) :
    mDataShape( shape ),
    mStart( mDataShape.rank() ),
    mStride( mDataShape.rank() ),
    mCount( mDataShape.rank() ) {
  }

  template< typename U > friend class Slab;
  template< typename U >
  Slab( const Slab<U>& other ) :
    mDataShape( other.mDataShape ),
    mStart( mDataShape.rank() ),
    mStride( mDataShape.rank() ),
    mCount( mDataShape.rank() ) {
    std::copy( 
      other.mStart.begin(), 
      other.mStart.end(),
      mStart.begin() );
    std::copy( 
      other.mStride.begin(), 
      other.mStride.end(),
      mStride.begin() );
    std::copy( 
      other.mCount.begin(), 
      other.mCount.end(),
      mCount.begin() );
  }

  ~Slab() {}

  void setStart( size_type dim, size_type start ) {
    mStart[dim] = start;
  }
  const size_type& start( size_type dim ) const {
    return mStart[dim];
  }
  
  void setStride( size_type dim, size_type stride ) {
    mStride[dim] = stride;
  }
  const size_type& stride( size_type dim ) const {
    return mStride[dim];
  }
  
  void setCount( size_type dim, size_type count ) {
    mCount[dim] = count;
  }
  const size_type& count( size_type dim ) const {
    return mCount[dim];
  }
};

template< typename T = std::size_t >
class SlabMap {
private:
  Slab<T> mFrom;
  Slab<T> mTo;

public:
  SlabMap( const Slab<T>& from, const Slab<T>& to ) :
    mFrom( from ),
    mTo( to )
  {}
  ~SlabMap() {}

  void setDomain( const Slab<T>& from ) {
    mFrom = from;
  }
  const Slab<T>& domain() const {
    return mFrom;
  }

  void setRange( const Slab<T>& to ) {
    mTo = to;
  }
  const Slab<T>& range() const {
    return mTo;
  }
};

XDM_NAMESPACE_END

#endif // xdm_Slab_hpp
