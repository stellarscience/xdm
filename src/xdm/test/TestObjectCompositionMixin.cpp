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
#define BOOST_TEST_MODULE ObjectCompositionMixin 
#include <boost/test/unit_test.hpp>

#include <xdm/ObjectCompositionMixin.hpp>

namespace {

using xdm::ReferencedObject;
using xdm::ObjectCompositionMixin;
using xdm::RefPtr;

struct A : ReferencedObject {};
struct B : ReferencedObject {};

class TestHarness : 
  public ObjectCompositionMixin< A >,
  public ObjectCompositionMixin< B >
{};

class ObjectCompositionMixinTest {
public:
	TestHarness t;
	RefPtr< A > a;
	RefPtr< B > b;
	ObjectCompositionMixinTest() :
		t(),
		a( new A ),
		b( new B ) {
		xdm::appendChild< A >( t, a );
		xdm::appendChild< B >( t, b );
	}
};

BOOST_AUTO_TEST_CASE( begin ) {
  ObjectCompositionMixinTest test;
	
  ObjectCompositionMixin< A >::Iterator it = xdm::begin< A >( test.t );
	BOOST_CHECK_EQUAL( test.a, *it );
	
  ObjectCompositionMixin< B >::Iterator bit = xdm::begin< B >( test.t );
	BOOST_CHECK_EQUAL( test.b, *bit );
}

BOOST_AUTO_TEST_CASE( child ) {
  ObjectCompositionMixinTest test;

	BOOST_CHECK_EQUAL( test.a, xdm::child< A >( test.t, 0 ) );
	BOOST_CHECK_EQUAL( test.b, xdm::child< B >( test.t, 0 ) );
}

} // namespace

