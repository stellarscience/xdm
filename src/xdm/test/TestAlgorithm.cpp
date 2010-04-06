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
#define BOOST_TEST_MODULE Algorithm
#include <boost/test/unit_test.hpp>

#include <xdm/Algorithm.hpp>

namespace {

BOOST_AUTO_TEST_CASE( equalPointerValue ) {
  int * a( new int( 42 ) );
  int * b( new int( 42 ) );

  BOOST_REQUIRE_NE( a, b );

  xdm::EqualPointerValue< int* > comp;
  BOOST_CHECK( comp( a, b ) );

  delete b;
  delete a;
}

BOOST_AUTO_TEST_CASE( orderedCollectionsEqual ) {
  std::vector< int > a;
  std::vector< int > b;

  a.push_back( 0 ); b.push_back( 0 );
  a.push_back( 1 ); b.push_back( 1 );
  a.push_back( 2 ); b.push_back( 2 );

  BOOST_CHECK( xdm::orderedCollectionsEqual(
    a.begin(), a.end(),
    b.begin(), b.end() ) );
}

BOOST_AUTO_TEST_CASE( orderedCollectionsEqualDifferentSize ) {
  std::vector< int > a;
  std::vector< int > b;

  a.push_back( 0 ); b.push_back( 0 );
  a.push_back( 1 ); b.push_back( 1 );
  a.push_back( 2 ); b.push_back( 2 );

  b.push_back( 3 );

  BOOST_CHECK( !xdm::orderedCollectionsEqual(
    a.begin(), a.end(),
    b.begin(), b.end() ) );
}

BOOST_AUTO_TEST_CASE( orderedCollectionsEqualDifferentOrder ) {
  std::vector< int > a;
  std::vector< int > b;

  a.push_back( 0 ); b.push_back( 2 );
  a.push_back( 1 ); b.push_back( 0 );
  a.push_back( 2 ); b.push_back( 1 );

  BOOST_CHECK( !xdm::orderedCollectionsEqual(
    a.begin(), a.end(),
    b.begin(), b.end() ) );
}

BOOST_AUTO_TEST_CASE( unorderedCollectionsEqualDifferentSize ) {
  std::vector< int > a;
  std::vector< int > b;

  a.push_back( 0 ); b.push_back( 0 );
  a.push_back( 1 ); b.push_back( 1 );
  a.push_back( 2 ); b.push_back( 2 );

  b.push_back( 3 );

  BOOST_CHECK( !xdm::unorderedCollectionsEqual(
    a.begin(), a.end(),
    b.begin(), b.end() ) );
}

BOOST_AUTO_TEST_CASE( unorderedCollectionsEqualDifferentOrder ) {
  std::vector< int > a;
  std::vector< int > b;

  a.push_back( 0 ); b.push_back( 2 );
  a.push_back( 1 ); b.push_back( 0 );
  a.push_back( 2 ); b.push_back( 1 );

  BOOST_CHECK( xdm::unorderedCollectionsEqual(
    a.begin(), a.end(),
    b.begin(), b.end() ) );
}

BOOST_AUTO_TEST_CASE( unorderedCollectionsEqualDifferentElements ) {
  std::vector< int > a;
  std::vector< int > b;

  a.push_back( 0 ); b.push_back( 4 );
  a.push_back( 1 ); b.push_back( 0 );
  a.push_back( 2 ); b.push_back( 1 );

  BOOST_CHECK( !xdm::unorderedCollectionsEqual(
    a.begin(), a.end(),
    b.begin(), b.end() ) );
}

BOOST_AUTO_TEST_CASE( toUpper ) {
  const char * input = "abcdefghijklmnopqrstuvwxyz";
  const char * answer = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  std::string result( input );
  xdm::toUpper( result );
  BOOST_CHECK_EQUAL( result, answer );
}

} // namespace
