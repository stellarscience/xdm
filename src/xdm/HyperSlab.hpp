#ifndef xdm_HyperSlab_hpp
#define xdm_HyperSlab_hpp

#include <xdm/DataShape.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <cstdlib>

XDM_NAMESPACE_BEGIN

/// Sampled subset of a dataset.  The data is indexed by a start, stride, and
/// count in each dimension.  This class is templated on it's size type to
/// provide interoperbility with datasets that may use a different primitive
/// type to represent the size of a dimension.  Defaults to std::size_t
template< typename T = std::size_t >
class HyperSlab {
private:
  typedef std::vector< T > IndexArray;
  DataShape<T> mDataShape;
  IndexArray mStart;
  IndexArray mStride;
  IndexArray mCount;

public:
  /// The size representation for the slab.
  typedef T size_type;

  /// Default constructor initializes to an empty shape with no sampling.
  HyperSlab() :
    mDataShape() {
  }

  /// Initialization from a shape.  Initializes the start, stride, and count
  /// arrays to match the rank of the shape.
  HyperSlab( const DataShape<T>& shape ) :
    mDataShape( shape ),
    mStart( mDataShape.rank() ),
    mStride( mDataShape.rank() ),
    mCount( mDataShape.rank() ) {
  }

  /// Give privelaged access to HyperSlabs with a different size type.
  template< typename U > friend class HyperSlab;

  /// Initialize from another HyperSlab with a different fundamental size
  /// representation.
  template< typename U >
  HyperSlab( const HyperSlab<U>& other ) :
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

  /// Destructor.
  ~HyperSlab() {}

  /// Get the DataShape of the HyperSlab.
  const DataShape< T >& shape() const {
    return mDataShape;
  }

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

/// Defines a mapping from one HyperSlab to another.  Used to transform one
/// representation of the data in memory or on disk to another.
template< typename T = std::size_t >
class HyperSlabMap {
private:
  HyperSlab<T> mFrom;
  HyperSlab<T> mTo;

public:
  HyperSlabMap() :
    mFrom(),
    mTo()
  {}

  HyperSlabMap( const HyperSlab<T>& from, const HyperSlab<T>& to ) :
    mFrom( from ),
    mTo( to )
  {}
  ~HyperSlabMap() {}

  void setDomain( const HyperSlab<T>& from ) {
    mFrom = from;
  }
  const HyperSlab<T>& domain() const {
    return mFrom;
  }

  void setRange( const HyperSlab<T>& to ) {
    mTo = to;
  }
  const HyperSlab<T>& range() const {
    return mTo;
  }
};

XDM_NAMESPACE_END

#endif // xdm_HyperSlab_hpp
