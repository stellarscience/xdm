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

#include <iostream>
#include <locale>
#include <stdexcept>
#include <streambuf>
#include <vector>

#include <xdm/NamespaceMacro.hpp>

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

/// Exception signalling a buffer underrun error when getting characters from
/// a BinaryStreamBuffer. By default, a BinaryStreamBuffer does not
/// automatically sync to the source of characters. It is up to the specific
/// subclasses of BinaryStreamBuffer to decide how to handle this situation.
class BinaryStreamBufferUnderrun : public std::runtime_error {
public:
  BinaryStreamBufferUnderrun() :
    std::runtime_error( "Attempt to read beyond binary buffer bounds." ) {}
};

/// Standard stream buffer subclass to handle reading and writing buffered data
/// using standard streams. The purpose of this class is to provide default
/// implementations of the streambuf's protected virtual members that can be
/// reused by subclasses providing the actual buffering behavior. In addition
/// to the default implementations of the protected virtual members, this class
/// exposes a public interface for querying the underlying buffer.
///
/// This class does not manage its own buffer, but rather allows clients to
/// customize and manage their own buffer for efficiency purposes. Thus, clients
/// with knowledge of their own buffering requirements will be able to subclass
/// BasicBinaryStreamBuffer to optimize their own buffering.
class BasicBinaryStreamBuffer : public std::basic_streambuf< char > {
public:

  virtual ~BasicBinaryStreamBuffer();

  /// Get a pointer to the beginning of the underlying buffer.
  char * bufferStart();
  size_t bufferSize() const;

  /// Static member to return the EOF condition.
  static std::char_traits< char >::int_type eof() {
    return std::char_traits< char >::eof();
  }

protected:
  BasicBinaryStreamBuffer();

  /// Imbue is a no-op since this streambuf is supposed to maintain a binary
  /// representation.
  virtual void imbue( const std::locale& );

  /// Set the pointer and size of the controlled sequence buffer. A null pointer
  /// or size of 0 will force the stream to become unbuffered.
  /// @param buffer Pointer to the beginning of an allocated buffer.
  /// @param size Size of the allocated buffer.
  virtual std::streambuf * setbuf( char * buffer, std::streamsize size );

  /// Seek to a position in the buffer relative to the beginning, end, or
  /// current position.
  /// @param offset Offset from position.
  /// @param from Seek from beginning, end or current position.
  /// @param mode Modify the input or output pointer.
  /// @return New buffer location measured from the beginning on success, -1
  /// upon failure.
  virtual std::streampos seekoff(
    std::streamoff offset,
    std::ios::seekdir from,
    std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out );

  /// Seek to an absolute position measured from the beginning of the buffer.
  /// @param position Distance from the beginning of the buffer.
  /// @param mode Modify the input or output pointer.
  /// @return New buffer location measure from the beginning on success, -1
  /// upon failure.
  virtual std::streampos seekpos(
    std::streampos position,
    std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out );

  /// Synchronize the stream buffer with the controlled sequence (the actual
  /// thing we are buffering access to). The default implementation just resets
  /// the input and output pointers to the beginning of the buffers. It is up
  /// subclasses to determine what to do with the buffer.
  /// @return 0 upon success, any other value on failure.
  virtual int sync();

  /// Get a character in the case of an underflow without advancing the get
  /// pointer. An underflow occurs when a character is requested, but the get
  /// pointer has moved beyond the end of the underlying buffer. The default
  /// implementation throws an exception.
  /// @throw BinaryStreamBufferUnderrun
  /// @return The new character available at the get pointer position upon
  /// success, end of file otherwise.
  virtual int underflow();

  /// Get a character in the case of an underflow and advance the get pointer.
  /// The default implementation throws an exception.
  /// @throw BinaryStreamBufferUnderrun
  /// @return The new character available at the get pointer position upon
  /// success, end of file otherwise.
  virtual int uflow();

  /// Put a character back in the buffer in the case of backup underflow. This
  /// happens when sputbackc or sungetc are called and the get pointer is at
  /// the beginning of the internal buffer or when the character at the putback
  /// position does not match the character passed to sputbackc. The default
  /// implementation throws an exception.
  /// @throw BinaryStreamBufferUnderrun
  /// @return End of file on failure, something else on success
  virtual int pbackfail( int c = std::char_traits< char >::eof() );

  /// Write a character in the case of overflow. Overflow occurs when a write is
  /// requested, but the put pointer is past the end of the underlying buffer.
  /// The default implementation throws an exception.
  /// @throw BinaryStreamBufferOverrun
  /// @return End of file on failure, anything else on success.
  virtual int overflow( int c = std::char_traits< char >::eof() );

};

/// BasicBinaryStreamBuffer that manages and owns it's own storage using a
/// standard vector.
class BinaryStreamBuffer : public BasicBinaryStreamBuffer {
public:
  BinaryStreamBuffer( std::streamsize bufferSize ) :
    BasicBinaryStreamBuffer(),
    mBuffer( bufferSize ) {
    setbuf( &mBuffer[0], mBuffer.size() );
  }

  virtual ~BinaryStreamBuffer() {}

private:
  std::vector< char > mBuffer;
};

XDM_NAMESPACE_END

#endif // xdm_BinaryStreamBuffer_hpp

