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

namespace {

struct Fixture {
  xdm::BinaryStreamBuffer testBuffer;
  Fixture() : testBuffer( 512 ) {}
};

BOOST_AUTO_TEST_CASE( construct ) {
  Fixture test;

  BOOST_CHECK_EQUAL( 512, test.testBuffer.size() );
}

BOOST_AUTO_TEST_CASE( putgetc ) {
  Fixture test;

  test.testBuffer.sputc( 'a' );
  test.testBuffer.sputc( 'b' );
  test.testBuffer.sputc( 'c' );

  test.testBuffer.pubsync();

  char result;
  result = test.testBuffer.sgetc();
  BOOST_CHECK_EQUAL( 'a', result );
  
  result = test.testBuffer.sgetc();
  BOOST_CHECK_EQUAL( 'b', result );
  
  result = test.testBuffer.sgetc();
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

} // namespace

