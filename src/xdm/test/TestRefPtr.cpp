#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

class Derived : public xdm::ReferencedObject {};

BOOST_AUTO_TEST_CASE( templateAssignmentOperator ) {
  xdm::RefPtr< Derived > a( new Derived );
  xdm::RefPtr< xdm::ReferencedObject > b;
  b = a;
  ASSERT_EQ( a.get(), b.get() );
  ASSERT_EQ( 2, a->referenceCount() );
  ASSERT_EQ( 2, b->referenceCount() );
}

BOOST_AUTO_TEST_CASE( assignmentOperator ) {
  xdm::RefPtr< xdm::ReferencedObject > a( new xdm::ReferencedObject );
  xdm::RefPtr< xdm::ReferencedObject > b;
  b = a;
  ASSERT_EQ( a.get(), b.get() );
  ASSERT_EQ( 2, a->referenceCount() );
  ASSERT_EQ( 2, b->referenceCount() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

