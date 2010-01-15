#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

class Derived : public xdm::ReferencedObject {};

BOOST_AUTO_TEST_CASE( templateAssignmentOperator ) {
  xdm::RefPtr< Derived > a( new Derived );
  xdm::RefPtr< xdm::ReferencedObject > b;
  b = a;
  BOOST_CHECK_EQUAL( a.get(), b.get() );
  BOOST_CHECK_EQUAL( 2, a->referenceCount() );
  BOOST_CHECK_EQUAL( 2, b->referenceCount() );
}

BOOST_AUTO_TEST_CASE( assignmentOperator ) {
  xdm::RefPtr< xdm::ReferencedObject > a( new xdm::ReferencedObject );
  xdm::RefPtr< xdm::ReferencedObject > b;
  b = a;
  BOOST_CHECK_EQUAL( a.get(), b.get() );
  BOOST_CHECK_EQUAL( 2, a->referenceCount() );
  BOOST_CHECK_EQUAL( 2, b->referenceCount() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

