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
#define BOOST_TEST_MODULE BinaryOStream 
#include <boost/test/unit_test.hpp>

#include <xdm/BinaryOStream.hpp>

#include <memory>

namespace {

// define a test parameterized on the types we care about.
template< typename T >
class Fixture {
public:
  T value;
  xdm::BinaryStreamBuffer mBuf;
  xdm::BinaryOStream mOstr;
  Fixture() : value(), mBuf( 512 ), mOstr( &mBuf ) {}
};

template< typename T > void readValue() {
  Fixture< T > test;

  test.mOstr << test.value;
  
  T* result = reinterpret_cast< T* >( test.mBuf.bufferStart() );
  BOOST_CHECK_EQUAL( test.value, *result );
}

BOOST_AUTO_TEST_CASE( writeChar ) { readValue< char >(); }
BOOST_AUTO_TEST_CASE( writeShort ) { readValue< short >(); }
BOOST_AUTO_TEST_CASE( writeInt ) { readValue< int >(); }
BOOST_AUTO_TEST_CASE( writeLong ) { readValue< long >(); }

BOOST_AUTO_TEST_CASE( writeUShort ) { readValue< unsigned short >(); }
BOOST_AUTO_TEST_CASE( writeUInt ) { readValue< unsigned int >(); }
BOOST_AUTO_TEST_CASE( writeULong ) { readValue< unsigned long >(); }

BOOST_AUTO_TEST_CASE( writeFloat ) { readValue< float >(); }
BOOST_AUTO_TEST_CASE( writeDouble ) { readValue< double >(); }
BOOST_AUTO_TEST_CASE( writeLongDouble ) { readValue< long double >(); }

BOOST_AUTO_TEST_CASE( writeBool ) { readValue< bool >(); }

} // namespace

