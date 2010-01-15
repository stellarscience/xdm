#include <gtest/gtest.h>

#include <xdm/ObjectCompositionMixin.hpp>

using xdm::ReferencedObject;
using xdm::ObjectCompositionMixin;
using xdm::RefPtr;

struct A : ReferencedObject {};
struct B : ReferencedObject {};

class TestHarness : 
  public ObjectCompositionMixin< A >,
  public ObjectCompositionMixin< B >
{};

class ObjectCompositionMixinTest : public ::testing::Test {
public:
	TestHarness t;
	A* a;
	B* b;
	ObjectCompositionMixinTest() :
		t(),
		a( new A ),
		b( new B ) {
		xdm::appendChild< A >( t, a );
		xdm::appendChild< B >( t, b );
	}
};

TEST_F( ObjectCompositionMixinTest, begin ) {
	ObjectCompositionMixin< A >::Iterator it = xdm::begin< A >( t );
	ASSERT_EQ( a, *it );
	ObjectCompositionMixin< B >::Iterator bit = xdm::begin< B >( t );
	ASSERT_EQ( b, *bit );
}

TEST_F( ObjectCompositionMixinTest, child ) {
	ASSERT_EQ( a, xdm::child< A >( t, 0 ) );
	ASSERT_EQ( b, xdm::child< B >( t, 0 ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

