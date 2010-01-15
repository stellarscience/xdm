//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
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

