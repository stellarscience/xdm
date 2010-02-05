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
#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/BinaryStreamBuffer.hpp>

#include <algorithm>
#include <vector>

namespace {

struct Fixture {
  xdm::BinaryStreamBuffer testBuffer;
  Fixture() : testBuffer( 512 ) {}
};

// test harness to buffer small output chunks to a large array.
class BufferedArrayOutputStreamBuf : public xdm::BinaryStreamBuffer  {
public:
  std::vector< char > mData;
  std::vector< char >::size_type mLocation;

  // initialize with an empty output vector (for output).
  BufferedArrayOutputStreamBuf() :
    xdm::BinaryStreamBuffer( 4 ),
    mData(),
    mLocation( 0 ) {}

  // initialize with a vector for buffered input.
  BufferedArrayOutputStreamBuf( const std::vector< char >& data ) :
    xdm::BinaryStreamBuffer( 4 ),
    mData( data ),
    mLocation( 0 ) {
      loadNextBlockIntoBuffer();
    }

  virtual ~BufferedArrayOutputStreamBuf() {}

  bool loadNextBlockIntoBuffer() {
    typedef std::vector< char >::difference_type difference_type;

    size_t loadCount = std::min(
      egptr() - eback(),
      difference_type( mData.size() ) - difference_type( mLocation ) );

    if ( loadCount ) {
      // there is more data to load
      std::copy( &mData[mLocation], &mData[mLocation + loadCount], eback() );
      mLocation += loadCount;
      return true;
    }
    return false;
  }

protected:

  virtual int overflow( int c = eof() ) {
    std::streamsize bufferContentSize = pptr() - pbase();

    // If there is content in the buffer, synchronize and return EOF on fail.
    if ( bufferContentSize && sync() ) {
      // sync return !0 on failure.
      return eof();
    }

    // Sync was successful. Begin writing to the beginning of the buffer again.
    if ( c != eof() ) {
      return sputc( c );
    }

    return eof();
  }

  virtual std::streamsize showmanyc()
  {
    return ( mData.size() - mLocation );
  }

  virtual int uflow()
  {
    std::streamsize bufferSize = egptr() - eback();

    if ( eback() && bufferSize ) {
      // load another block of data into the buffer.
      if ( loadNextBlockIntoBuffer() ) {
        gbump( -bufferSize );
        return sbumpc();
      }
    }
    return eof();
  }

  virtual int sync() {
    std::streamsize bufferContentSize = pptr() - pbase();

    if ( bufferContentSize ) {
      // size the output data to fit
      mData.resize( mData.size() + bufferContentSize );

      // copy to the newly allocated output elements and increase location.
      std::copy( pbase(), pptr(), &mData[mLocation] );
      mLocation += bufferContentSize;

      // reset the put pointer back to the beginning of the buffer.
      pbump( -bufferContentSize );

      // Signal success.
      return 0;
    }
    return eof();
  }
};

BOOST_AUTO_TEST_CASE( construct ) {
  Fixture test;

  BOOST_CHECK_EQUAL( 512, test.testBuffer.bufferSize() );
}

BOOST_AUTO_TEST_CASE( putgetc ) {
  Fixture test;

  test.testBuffer.sputc( 'a' );
  test.testBuffer.sputc( 'b' );
  test.testBuffer.sputc( 'c' );

  test.testBuffer.pubsync();

  char result;
  result = test.testBuffer.sbumpc();
  BOOST_CHECK_EQUAL( 'a', result );
  
  result = test.testBuffer.sbumpc();
  BOOST_CHECK_EQUAL( 'b', result );
  
  result = test.testBuffer.sbumpc();
  BOOST_CHECK_EQUAL( 'c', result );
}

BOOST_AUTO_TEST_CASE( putgetn ) {
  Fixture test;

  char characters[] = {'a', 'b', 'c'};
  
  test.testBuffer.sputn( characters, 3 );

  test.testBuffer.pubsync();

  char result[3];
  test.testBuffer.sgetn( result, 3 );

  BOOST_CHECK_EQUAL( 'a', result[0] );
  BOOST_CHECK_EQUAL( 'b', result[1] );
  BOOST_CHECK_EQUAL( 'c', result[2] );
}

BOOST_AUTO_TEST_CASE( bufferOverrun ) {
  xdm::BinaryStreamBuffer test( 4 );
  test.sputc( 'a' );
  test.sputc( 'b' );
  test.sputc( 'c' );
  test.sputc( 'd' );
  BOOST_CHECK_THROW( test.sputc( 'e' ), xdm::BinaryStreamBufferOverrun );
}

BOOST_AUTO_TEST_CASE( bufferedOutput ) {
  BufferedArrayOutputStreamBuf test;
  std::vector< char > answer;
  for ( char c = 'a'; c < 'z'; c++ ) {
    test.sputc( c );
    answer.push_back( c );
  }

  // one last sync on the way out to flush the buffer
  test.pubsync();

  BOOST_CHECK_EQUAL_COLLECTIONS( answer.begin(), answer.end(),
    test.mData.begin(), test.mData.end() );
}

BOOST_AUTO_TEST_CASE( bufferedInput ) {
  std::vector< char > answer;
  for ( char c = 'a'; c < 'z'; c++ ) {
    answer.push_back( c );
  }

  BufferedArrayOutputStreamBuf test( answer );

  std::vector< char > result;
  for ( int i = 0; i < 25; i++ ) {
    result.push_back( test.sbumpc() );
  }
  BOOST_CHECK_EQUAL_COLLECTIONS( answer.begin(), answer.end(),
    result.begin(), result.end() );
}

} // namespace

