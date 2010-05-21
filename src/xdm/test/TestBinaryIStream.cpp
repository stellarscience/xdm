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
#define BOOST_TEST_MODULE BinaryIStream 
#include <boost/test/unit_test.hpp>

#include <xdm/BinaryIStream.hpp>

#include <memory>

namespace {

// define a test parameterized on the types we care about.
template< typename T >
class Fixture {
public:
  T value;
  xdm::BinaryStreamBuffer mBuf;
  xdm::BinaryIStream mIStr;
  Fixture() : value(), mBuf( 512 ), mIStr( &mBuf ) {
    mBuf.sputn( reinterpret_cast< char* >(&value), sizeof(T) );
    mBuf.pubsync();
  }
};

// Workaround Run-Time Check Failure #3 from VC8.
template< typename Type > Type make() {
  union Workaround {
    Type type;
    double double_;
  };
  return Workaround().type;
}

template< typename T > void writeValue() {
  Fixture< T > test;

  T result = make< T >();
  // make sure result isn't the default object
  result = !result;

  test.mIStr >> result;
  BOOST_CHECK_EQUAL( test.value, result );
}

BOOST_AUTO_TEST_CASE( writeChar ) { writeValue< char >(); }
BOOST_AUTO_TEST_CASE( writeShort ) { writeValue< short >(); }
BOOST_AUTO_TEST_CASE( writeInt ) { writeValue< int >(); }
BOOST_AUTO_TEST_CASE( writeLong ) { writeValue< long >(); }

BOOST_AUTO_TEST_CASE( writeUShort ) { writeValue< unsigned short >(); }
BOOST_AUTO_TEST_CASE( writeUInt ) { writeValue< unsigned int >(); }
BOOST_AUTO_TEST_CASE( writeULong ) { writeValue< unsigned long >(); }

BOOST_AUTO_TEST_CASE( writeFloat ) { writeValue< float >(); }
BOOST_AUTO_TEST_CASE( writeDouble ) { writeValue< double >(); }
BOOST_AUTO_TEST_CASE( writeLongDouble ) { writeValue< long double >(); }

BOOST_AUTO_TEST_CASE( writeBool ) { writeValue< bool >(); }

} // namepsace anon

