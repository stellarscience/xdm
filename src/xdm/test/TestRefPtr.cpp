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
#define BOOST_TEST_MODULE RefPtr
#include <boost/test/unit_test.hpp>

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

namespace {

class Derived : public xdm::ReferencedObject {};
class AlsoDerived : public xdm::ReferencedObject {};

void takesBaseClass( xdm::RefPtr< xdm::ReferencedObject > obj ) {
  BOOST_CHECK( true );
}

class TestBase : public xdm::ReferencedObject {
public:
  TestBase( int n ) : mInt( n ) {}
  int getInt() const { return mInt; }
private:
  int mInt;
};

class TestDerived : public TestBase {
public:
  TestDerived( int n1, int n2 ) : TestBase( n1 ), mInt2( n2 ) {}
  int getInt() const { return mInt2; } // Hides base class getInt
  int getIntBase() const { return TestBase::getInt(); }
private:
  int mInt2;
};

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

BOOST_AUTO_TEST_CASE( dynamicCast ) {
  xdm::RefPtr< xdm::ReferencedObject > ptr( new Derived );
  xdm::RefPtr< Derived > derived = xdm::dynamic_pointer_cast< Derived >( ptr );
  BOOST_CHECK( derived.valid() );

  xdm::RefPtr< AlsoDerived > alsoDerived = xdm::dynamic_pointer_cast< AlsoDerived >( ptr );
  BOOST_CHECK( !alsoDerived.valid() );
}

BOOST_AUTO_TEST_CASE( implicitConversionToBase ) {
  xdm::RefPtr< Derived > ptr;
  takesBaseClass( ptr );
}

BOOST_AUTO_TEST_CASE( dynamicCast2 ) {
  xdm::RefPtr< TestBase > basePtr( new TestDerived( 1, 2 ) );
  xdm::RefPtr< TestDerived > derivedPtr = xdm::dynamic_pointer_cast< TestDerived >( basePtr );
  BOOST_REQUIRE( derivedPtr.valid() );

  // Check member access.
  BOOST_CHECK_EQUAL( 1, derivedPtr->getIntBase() );
  BOOST_CHECK_EQUAL( 2, derivedPtr->getInt() );
}

} // namespace

