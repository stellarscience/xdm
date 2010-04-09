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
#define BOOST_TEST_MODULE DataShape 
#include <boost/test/unit_test.hpp>

#include <xdm/DataShape.hpp>

#include <sstream>
#include <stdexcept>

#include <iostream>

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

BOOST_AUTO_TEST_CASE( contiguousIndexFromShape ) {
  // The answer is a 3x3x2 array.
  std::size_t answer[3][3][2];
  std::size_t index = 0;
  for ( int i = 0; i < 3; ++i ) {
    for ( int j = 0; j < 3; ++j ) {
      for ( int k = 0; k < 2; ++k ) {
        answer[i][j][k] = index++;
      }
    }
  }

  xdm::DataShape<> context( 3 );
  context[0] = 3;
  context[1] = 3;
  context[2] = 2;

  for ( int i = 0; i < 3; ++i ) {
    for ( int j = 0; j < 3; ++j ) {
      for ( int k = 0; k < 2; ++k ) {
        BOOST_CHECK_EQUAL(
          contiguousIndex( xdm::makeShape( i, j, k ), context ),
          answer[i][j][k] );
      }
    }
  }
}

} // namespace

