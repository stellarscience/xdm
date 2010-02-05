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
#include <xdm/BinaryStreamBuffer.hpp>

#include <utility>

#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

BasicBinaryStreamBuffer::BasicBinaryStreamBuffer() :
  std::basic_streambuf< char >()
{
}

BasicBinaryStreamBuffer::~BasicBinaryStreamBuffer()
{
}

char * BasicBinaryStreamBuffer::bufferStart()
{
  return eback();
}

std::size_t BasicBinaryStreamBuffer::bufferSize() const
{
  return egptr() - eback();
}

void BasicBinaryStreamBuffer::imbue( const std::locale& )
{
  // no-op
}

std::streambuf * BasicBinaryStreamBuffer::setbuf(
  char * buffer,
  std::streamsize size )
{
  // set up both the input and output buffers.
  setg( buffer, buffer, buffer + size );
  setp( buffer, buffer + size );
  return this;
}

std::streampos BasicBinaryStreamBuffer::seekoff(
  std::streamoff offset,
  std::ios::seekdir from,
  std::ios_base::openmode mode )
{
  // Default to returning failure.
  std::streampos returnValue( -1 );
  std::streamsize bufferSize = egptr() - eback();

  // Determine if we are changing the input stream, the output stream, or both.
  bool changeIn = ( std::ios_base::in & mode ) != 0;
  bool changeOut = ( std::ios_base::out & mode ) != 0;
  bool changeBoth = changeIn && changeOut && ( from != std::ios::cur );

  // Choose the begin pointer based on what is being modified (although it
  // doesn't really matter as they share the same buffer).
  const char * begin = changeIn ? eback() : pbase();

  // Perform the operation only if the buffer is not null, the offset is not
  // zero, and we are modifying something.
  if ( ( begin || !offset ) && ( changeIn || changeOut || changeBoth ) ) {
    // offsets are measured from the beginning of the buffer.
    std::streamoff offsetIn = offset;
    std::streamoff offsetOut = offset;

    // in the case that the requested offset is not from the beginning, modify
    // the input buffer and output buffer offsets so they can be measured from
    // the beginning of the buffer.
    if ( from == std::ios_base::cur ) {
      offsetIn += gptr() - begin;
      offsetOut += pptr() - begin;
    } else if ( from == std::ios_base::end ) {
      offsetIn += egptr() - begin;
      offsetOut = offsetIn;
    }

    // If changing the input pointer, input offset is valid and we will not
    // overrun the buffer, then change the get pointer for input.
    if (
      (changeIn || changeBoth) &&
      offsetIn >= 0 &&
      bufferSize >= offsetIn ) {
      gbump( (begin + offsetIn) - gptr() );
      returnValue = std::streampos( offsetIn );
    }

    // If changing the output pointer, the output offset is valid, and we will
    // overrun the buffer, then change the put pointer.
    if (
      (changeOut || changeBoth) &&
      offsetOut >= 0 &&
      bufferSize >= offsetOut ) {
      pbump( (begin + offsetOut) - pptr() );
      returnValue = std::streampos( offsetOut );
    }
  }

  return returnValue;
}

std::streampos BasicBinaryStreamBuffer::seekpos(
  std::streampos position,
  std::ios_base::openmode mode )
{
  // default to return failure.
  std::streampos returnValue( -1 );
  std::streamoff positionAsOffset( position );
  std::streamsize bufferSize = egptr() - eback();

  // determine which pointers to modify
  bool changeIn = ( std::ios_base::in & mode ) != 0;
  bool changeOut = ( std::ios_base::out & mode ) != 0;

  const char * begin = changeIn ? eback() : pbase();

  // perform the operation only if the buffer is valid, the position is non-zero
  // and something needs to be changed.
  if ( ( begin || !positionAsOffset ) && ( changeIn || changeOut ) ) {
    bool validPosition = ( 0 <= positionAsOffset && positionAsOffset <= bufferSize );

    if ( validPosition ) {
      if ( changeIn ) {
        pbump( (begin + positionAsOffset) - gptr() );
      }
      if ( changeOut ) {
        gbump( (begin + positionAsOffset) - pptr() );
      }
      returnValue = position;
    }
  }
  return returnValue;
}

int BasicBinaryStreamBuffer::sync()
{
  // Determine each pointer's offset from the beginning of the buffer.
  std::streamoff putOffset = pptr() - pbase();
  std::streamoff getOffset = gptr() - eback();

  // bump the output pointer back to the beginning of the buffer.
  pbump( -putOffset );
  // bump the input pointer back to the beginning of the buffer.
  gbump( -getOffset );
  // Signal success.
  return 0;
}

int BasicBinaryStreamBuffer::underflow()
{
  XDM_THROW( BinaryStreamBufferUnderrun() );
  return BasicBinaryStreamBuffer::eof();
}

int BasicBinaryStreamBuffer::uflow()
{
  XDM_THROW( BinaryStreamBufferUnderrun() );
  return BasicBinaryStreamBuffer::eof();
}

int BasicBinaryStreamBuffer::pbackfail( int )
{
  XDM_THROW( BinaryStreamBufferUnderrun() );
  return BasicBinaryStreamBuffer::eof();
}

int BasicBinaryStreamBuffer::overflow( int )
{
  XDM_THROW( BinaryStreamBufferOverrun() );
  return BasicBinaryStreamBuffer::eof();
}

XDM_NAMESPACE_END
