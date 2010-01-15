//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.      
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
#define BOOST_TEST_MODULE BinaryIOStream 
#include <boost/test/unit_test.hpp>

#include <xdmComm/BinaryIOStream.hpp>

#include <memory>

namespace {

// define a test parameterized on the types we care about.
template< typename T >
class Fixture {
public:
  xdmComm::BinaryStreamBuffer mBuf;
  xdmComm::BinaryIOStream mIOStr;
  Fixture() : mBuf( 512 ), mIOStr( &mBuf ) {
  }
};

template< typename T > void writeReadTest() {
  Fixture< T > test;
  
  T answer = T();

  test.mIOStr << answer;

  test.mIOStr.flush();

  T result = T();
  result = !result; // ensure result is not default value for a good test
  test.mIOStr >> result;

  BOOST_CHECK_EQUAL( answer, result );
}

BOOST_AUTO_TEST_CASE( writeReadChar ) { writeReadTest< char >(); }
BOOST_AUTO_TEST_CASE( writeReadShort ) { writeReadTest< short >(); }
BOOST_AUTO_TEST_CASE( writeReadInt ) { writeReadTest< int >(); }
BOOST_AUTO_TEST_CASE( writeReadLong ) { writeReadTest< long >(); }

BOOST_AUTO_TEST_CASE( writeReadUShort ) { writeReadTest< unsigned short >(); }
BOOST_AUTO_TEST_CASE( writeReadUInt ) { writeReadTest< unsigned int >(); }
BOOST_AUTO_TEST_CASE( writeReadULong ) { writeReadTest< unsigned long >(); }

BOOST_AUTO_TEST_CASE( writeReadFloat ) { writeReadTest< float >(); }
BOOST_AUTO_TEST_CASE( writeReadDouble ) { writeReadTest< double >(); }
BOOST_AUTO_TEST_CASE( writeReadLongDouble ) { writeReadTest< long double >(); }

BOOST_AUTO_TEST_CASE( writeReadBool ) { writeReadTest< bool >(); }

} // namespace

