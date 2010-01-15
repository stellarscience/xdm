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
#ifndef xdm_BinaryStreamBuffer_hpp
#define xdm_BinaryStreamBuffer_hpp

#include <memory>
#include <stdexcept>

#include <xdm/NamespaceMacro.hpp>
#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Exception signalling a buffer overrun error when filling a
/// BinaryStreamBuffer. A BinaryStreamBuffer does not fill and output as a
/// std::streambuf would. An explicit call to BinaryStreamBuffer::pubsync()
/// is required to flush the buffer occasionally to empty the buffer. This
/// exception should be raised when the buffer is overrun.
class BinaryStreamBufferOverrun : public std::runtime_error {
public:
  BinaryStreamBufferOverrun() :
    std::runtime_error( "Attempt to place object in a full binary buffer." ) {}
};

/// Class to handle reading and writing binary data to/from a buffer.  The I/O
/// functions in this buffer handle the management of a buffer pointer so that
/// items may be read/written incrementally to a stream.  The interface of this
/// class is designed to mimic that of a std::streambuf so that we may use that
/// someday.
///
/// This class does not fill and output as a std::streambuf would.  An explicit
/// call to sync() must be called to flush the buffer occasionally, or it will
/// fill up.  Attempts to put new data into a full buffer will result in thrown
/// exceptions.
template< typename AllocT = std::allocator<char> >
class BasicBinaryStreamBuffer {
private:
  size_t mSize;
  char* mData;
  char* mLocation;

  AllocT mAlloc;

public:

  BasicBinaryStreamBuffer( size_t size ) :
    mSize( size ),
    mData( mAlloc.allocate( size ) ),
    mLocation( mData ) {}

  virtual ~BasicBinaryStreamBuffer() {
    mAlloc.deallocate( mData, mSize );
  }

  /// Put a single character into the buffer at the current location.
  /// @throw BinaryStreamBufferOverrun The data to place in the buffer is larger
  /// than the available buffer space.
  void sputc( char c ) { 
    if ( mData + mSize < mLocation + 1 ) {
      XDM_THROW( BinaryStreamBufferOverrun() );
    }
    *mLocation++ = c; 
  }

  /// Put a character sequence into the buffer at the current location.
  /// @throw BinaryStreamBufferOverrun The data to place in the buffer is larger
  /// than the available buffer space.
  void sputn( const char* in, size_t n ) {
    if ( mData + mSize < mLocation + n ) {
      XDM_THROW( BinaryStreamBufferOverrun() );
    }
    std::uninitialized_copy( in, in + n, mLocation );
    mLocation += n;
  }

  /// Get a single character from the buffer at the current location.
  /// @throw BinaryStreamBufferOverrun The data to place in the buffer is larger
  /// than the available buffer space.
  char sgetc() {
    if ( mData + mSize < mLocation ) {
      XDM_THROW( BinaryStreamBufferOverrun() );
    }
    return *mLocation++;
  }

  /// Get a sequence of characters from the buffer at the current location.
  /// @throw BinaryStreamBufferOverrun The data to place in the buffer is larger
  /// than the available buffer space.
  void sgetn( char* out, size_t n ) {
    if ( mData + mSize < mLocation + n ) {
      XDM_THROW( BinaryStreamBufferOverrun() );
    }
    std::uninitialized_copy( mLocation, mLocation + n, out );
    mLocation += n;
  }

  /// Get the allocated size of the buffer.
  size_t size() const {
    return mSize;
  }

  /// Get the raw pointer to the beginning of the buffer.
  char* pointer() {
    return mData;
  }

  /// Seek to an absolute position in the buffer
  size_t pubseekpos( size_t position ) {
    mLocation = mData + position;
    return position;
  }

  /// Reset the location pointer to the beginning of the buffer.
  void pubsync() {
    sync();
  }

protected:
  virtual void sync() {
    mLocation = mData;
  }
};

/// BinaryStreamBuffer with a standard allocator.
typedef BasicBinaryStreamBuffer<> BinaryStreamBuffer;

XDM_NAMESPACE_END

#endif // xdm_BinaryStreamBuffer_hpp

