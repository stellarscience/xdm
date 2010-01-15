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
#ifndef xdm_HyperSlabBlockIterator_hpp
#define xdm_HyperSlabBlockIterator_hpp

#include <xdm/DataShape.hpp>
#include <xdm/HyperSlab.hpp>

#include <algorithm>
#include <iterator>

#include <cassert>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

namespace detail {
  template< typename T > struct Minimum {
    const T& operator()( const T& lhs, const T& rhs ) {
      return std::min( lhs, rhs );
    }
  };
} // namespace detail 

/// Break a HyperSlab into a set of blocks with a given shape and iterate
/// through the blocks.  This class defines an input iterator from which a
/// sequence of HyperSlabs may be read.
template< typename T = size_t >
class HyperSlabBlockIterator {
private:

  typedef HyperSlabBlockIterator< T > Self;
  typedef typename HyperSlab< T >::size_type SizeType;

  HyperSlab< T > mCompleteSlab;
  DataShape< T > mBlockSize;
  HyperSlab< T > mCurrentSlab;

  void advance( SizeType dimension ) {
    if ( dimension < mBlockSize.rank() ) {
      
      // begin by setting the start, stride, and count to reflect those of the
      // current slab.
      SizeType start = mCurrentSlab.start( dimension );
      SizeType stride = mCurrentSlab.stride( dimension );
      SizeType count = mCurrentSlab.count( dimension );
     
      // advance the starting point to cover count strides
      start += stride * count;
      // check to see if we have reached the end in this dimension
      if ( start >= mCompleteSlab.finalIndex( dimension ) ) {
        // roll back to the start index for this dimension
        mCurrentSlab.setStart( dimension, mCompleteSlab.start( dimension ) );
        // update the count, handle overlap condition.
        mCurrentSlab.setCount( dimension, 
          std::min( 
            mCompleteSlab.count( dimension ), 
            mBlockSize[dimension] ) );
        // advance the next dimension
        advance( dimension + 1 );
      } else {
        // still have more to go in this dimension
        mCurrentSlab.setStart( dimension, start );
        count = mCompleteSlab.finalIndex( dimension ) - start;
        count /= mCompleteSlab.stride( dimension );
        mCurrentSlab.setCount( dimension, 
          std::min( count, mBlockSize[dimension] ) );
      }
    } else {
      // we have reached the end, invalidate the iterator.
      mCompleteSlab = xdm::HyperSlab<>();
      mBlockSize = xdm::DataShape<>();
      mCurrentSlab = xdm::HyperSlab<>();
    }
  }

public:

  /// This class defines an input iterator
  typedef std::input_iterator_tag iterator_category;
  /// Value type is a HyperSlab
  typedef HyperSlab< T > value_type;
  /// Pointer to the value_type 
  typedef const HyperSlab< T >* pointer;
  /// Reference type is const, returned slabs are immutable
  typedef const HyperSlab< T >& reference;
  /// Difference type is signed
  typedef long difference_type;

  /// Default constructor makes a singular iterator.  Assignment is the only
  /// operation that is not undefined.
  HyperSlabBlockIterator() :
    mCompleteSlab(),
    mBlockSize(),
    mCurrentSlab() {
  }

  /// Construct an iterator over the input slab with the given shape.  The
  /// iterator will be initialized to point at the beginning of the input slab.
  HyperSlabBlockIterator(
    const HyperSlab< T >& completeSlab,
    const DataShape< T >& blockSize ) :
    mCompleteSlab( completeSlab ),
    mBlockSize( blockSize ),
    mCurrentSlab( mBlockSize ) {
    
    assert( mBlockSize.rank() == mCompleteSlab.shape().rank() );
    
    using std::copy;
    using std::transform;

    // start at the beginning of the complete slab.
    copy( mCompleteSlab.beginStart(), mCompleteSlab.endStart(),
      mCurrentSlab.beginStart() );
    // stride is the same as the complete slab.
    copy( mCompleteSlab.beginStride(), mCompleteSlab.endStride(),
      mCurrentSlab.beginStride() );
    // count is the minimum of the block size in each dimension and the count
    // for that dimension in the complete slab.
    transform( 
      mBlockSize.begin(), 
      mBlockSize.end(), 
      mCompleteSlab.beginCount(),
      mCurrentSlab.beginCount(),
      detail::Minimum< SizeType >() );
  }

  ~HyperSlabBlockIterator() {}

  /// Copy constructor copies the complete slab, the block size, and the current
  /// location.
  HyperSlabBlockIterator( const Self& rhs ) :
    mCompleteSlab( rhs.mCompleteSlab ),
    mBlockSize( rhs.mBlockSize ),
    mCurrentSlab( rhs.mCurrentSlab ) {
  }

  /// Assignment operator.
  Self& operator=( const Self& rhs ) {
    mCompleteSlab = rhs.mCompleteSlab;
    mBlockSize = rhs.mBlockSize;
    mCurrentSlab = rhs.mCurrentSlab;
    return *this;
  }

  /// Preincrement (++i).
  reference operator++() {
    advance( 0 );
    return mCurrentSlab;
  }

  /// Post increment (i++).
  value_type operator++( int ) {
    HyperSlab< T > tmp = mCurrentSlab;
    advance( 0 );
    return tmp;
  }

  /// Dereference 
  reference operator*() {
    return mCurrentSlab;
  }

  /// Member access
  pointer operator->() {
    return &mCurrentSlab;
  }

  /// Iterators are equal iff the complete slab, block size, and current slab
  /// are the same.
  bool operator==( const Self& rhs ) const {
    return 
      ( mCompleteSlab == rhs.mCompleteSlab ) && 
      ( mBlockSize == rhs.mBlockSize ) &&
      ( mCurrentSlab == rhs.mCompleteSlab );
  }

  /// Iterators are not equal iff the complete slab, block size, or current slab
  /// are not the same.
  bool operator!=( const Self& rhs ) const {
    return !operator==( rhs );
  }
};

XDM_NAMESPACE_END

#endif // xdm_HyperSlabBlockIterator_hpp

