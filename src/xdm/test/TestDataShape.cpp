//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE DataShape 
#include <boost/test/unit_test.hpp>

#include <xdm/DataShape.hpp>

#include <sstream>
#include <stdexcept>

namespace {

typedef xdm::DataShape<> TestShape;

BOOST_AUTO_TEST_CASE( OStreamInserter ) {
  TestShape test( 4 );
  for ( int i = 0; i < 4; ++i ) {
    test[i] = i+1;
  }

  char const * const answer = "1 2 3 4";
  std::stringstream result;
  result << test;
  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( makeShape ) {
  TestShape result = xdm::makeShape( "1 2 3 4" );
  BOOST_CHECK_EQUAL( 4, result.rank() );
  BOOST_CHECK_EQUAL( 1, result[0] );
  BOOST_CHECK_EQUAL( 2, result[1] );
  BOOST_CHECK_EQUAL( 3, result[2] );
  BOOST_CHECK_EQUAL( 4, result[3] );

  BOOST_CHECK_THROW( xdm::makeShape( "1 jeff" ), std::invalid_argument );
}

} // namespace

