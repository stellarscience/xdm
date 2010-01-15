#ifndef xdm_HyperSlab_hpp
#define xdm_HyperSlab_hpp

#include <xdm/DataShape.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <iterator>
#include <ostream>

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

  void updateShape() {
    mStart.resize( mDataShape.rank() );
    mStride.resize( mDataShape.rank() );
    mCount.resize( mDataShape.rank() );
  }

public:
  /// The size representation for the slab.
  typedef T size_type;

  /// Iterators to iterate through slab information.
  typedef typename IndexArray::iterator iterator;
  typedef typename IndexArray::const_iterator const_iterator;

  /// Default constructor initializes to an empty shape with no sampling.
  HyperSlab() :
    mDataShape(),
    mStart( mDataShape.rank() ),
    mStride( mDataShape.rank() ),
    mCount( mDataShape.rank() ) {
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

  /// Set the DataShape of the HyperSlab.  This will resize the start, stride,
  /// and count elements of the slab so that their size matches the rank of the
  /// shape.
  void setShape( const DataShape< T >& shape ) {
    mDataShape = shape;
    updateShape();
  }

  /// Get the DataShape of the HyperSlab.
  const DataShape< T >& shape() const {
    return mDataShape;
  }

  size_type size() const {
    size_type result = 1;
    for ( size_type i = 0; i < mCount.size(); i++ ) {
      result *= ( mCount[i] / mStride[i] );
    }
    return result;
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

  //-- Slab information iterator interface --//
  iterator beginStart() { return mStart.begin(); }
  const_iterator beginStart() const { return mStart.begin(); }
  iterator endStart() { return mStart.end(); }
  const_iterator endStart() const { return mStart.end(); }
  
  iterator beginStride() { return mStride.begin(); }
  const_iterator beginStride() const { return mStride.begin(); }
  iterator endStride() { return mStride.end(); }
  const_iterator endStride() const { return mStride.end(); }
  
  iterator beginCount() { return mCount.begin(); }
  const_iterator beginCount() const { return mCount.begin(); }
  iterator endCount() { return mCount.end(); }
  const_iterator endCount() const { return mCount.end(); }

  /// Reverse the dimension ordering for the hyperslab.
  /// @see DataShape::reverseDimensionOrder
  void reverseDimensionOrder() {
    mDataShape.reverseDimensionOrder();
    std::reverse( beginStart(), endStart() );
    std::reverse( beginStride(), endStride() );
    std::reverse( beginCount(), endCount() );
  }
};

template< typename T >
bool operator==( const HyperSlab< T >& lhs, const HyperSlab< T >& rhs ) {
  if ( lhs.shape() != rhs.shape() ) {
    return false;
  }
  return (
    std::equal( lhs.beginStart(), lhs.endStart(), rhs.beginStart() ) &&
    std::equal( lhs.beginStride(), lhs.endStride(), rhs.beginStride() ) &&
    std::equal( lhs.beginCount(), lhs.endCount(), rhs.beginCount() ) 
  ); 
}

/// Reverse the dimension ordering for a hyperslab.
/// @see DataShape::reverseDimensionOrder
template< typename T >
void reverseDimensionOrder( HyperSlab< T >& hyperSlab ) {
  hyperSlab.reverseDimensionOrder();
}

template< typename T >
std::ostream& operator<<( std::ostream& ostr, const HyperSlab<T>& slab ) {
  typedef typename HyperSlab<T>::size_type SizeType;
  // start
  ostr << "( ";
  std::copy( slab.beginStart(), slab.endStart(), 
    std::ostream_iterator< SizeType >( ostr, " " ) );
  ostr << " )";
  
  // stride
  ostr << "( ";
  std::copy( slab.beginStride(), slab.endStride(), 
    std::ostream_iterator< SizeType >( ostr, " " ) );
  ostr << " )";
  
  // count
  ostr << "( ";
  std::copy( slab.beginCount(), slab.endCount(), 
    std::ostream_iterator< SizeType >( ostr, " " ) );
  ostr << " )";
  return ostr;
}

XDM_NAMESPACE_END

#endif // xdm_HyperSlab_hpp

