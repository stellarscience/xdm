//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE RefPtr 
#include <boost/test/unit_test.hpp>

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

namespace {

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

} // namespace

