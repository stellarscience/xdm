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
#define BOOST_TEST_MODULE TemplateStructuredArray
#include <boost/test/unit_test.hpp>

#include <xdm/DataShape.hpp>
#include <xdm/TemplateStructuredArray.hpp>
#include <xdm/RefPtr.hpp>

namespace {

template< typename T > 
void test() {
  T value = T();
  xdm::RefPtr< xdm::TemplateStructuredArray< T > > array( 
    new xdm::TemplateStructuredArray< T >( &value, 1 ) );
  T* result = array->begin();
  BOOST_CHECK_EQUAL( result, &value );
}

BOOST_AUTO_TEST_CASE( instantiateChar ) { test< char >(); } 
BOOST_AUTO_TEST_CASE( instantiateShort ) { test< short >(); } 
BOOST_AUTO_TEST_CASE( instantiateInt ) { test< int >(); } 
BOOST_AUTO_TEST_CASE( instantiateLongInt ) { test< long int >(); } 

BOOST_AUTO_TEST_CASE( instantiateUChar ) { test< unsigned char >(); } 
BOOST_AUTO_TEST_CASE( instantiateUShort ) { test< unsigned short >(); } 
BOOST_AUTO_TEST_CASE( instantiateUInt ) { test< unsigned int >(); } 
BOOST_AUTO_TEST_CASE( instantiateULInt ) { test< long unsigned int >(); } 

BOOST_AUTO_TEST_CASE( instantiateFloat ) { test< float >(); } 
BOOST_AUTO_TEST_CASE( instantiateDouble ) { test< double >(); } 

} // namespace

