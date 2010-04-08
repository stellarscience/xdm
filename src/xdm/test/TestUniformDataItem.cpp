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
#define BOOST_TEST_MODULE UniformDataItem
#include <boost/test/unit_test.hpp>

#include <xdm/test/TestHelpers.hpp>

#include <xdm/TypedStructuredArray.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/VectorStructuredArray.hpp>
#include <xdm/ArrayAdapter.hpp>

#include <algorithm>

namespace {

xdm::RefPtr< xdm::UniformDataItem > createData( int sizex, int sizey, int sizez ) {
  xdm::RefPtr< xdm::UniformDataItem > result( new xdm::UniformDataItem );
  result->setDataspace( xdm::makeShape( sizex, sizey, sizez ) );
  result->setDataType( xdm::primitiveType::kInt );
  xdm::RefPtr< xdm::VectorStructuredArray< int > > array(
    new xdm::VectorStructuredArray< int >( sizex*sizey*sizez ) );
  for ( int i = 0; i < sizex; i++ ) {
    for ( int j = 0; j < sizey; j++ ) {
      for ( int k = 0; k < sizez; k++ ) {
        int index = i*sizey*sizez + j*sizez + k;
        (*array)[index] = index;
      }
    }
  }
  result->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( array ) ) );
  return result;
}

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  test::Fixture test;

  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );
  test.testItem->writeMetadata( xml );

  BOOST_CHECK_EQUAL( xml.tag(), "DataItem" );
  BOOST_CHECK_EQUAL( xml.attribute( "ItemType" ), "Uniform" );
  BOOST_CHECK_EQUAL( xml.attribute( "Dimensions" ), "2 2" );
  BOOST_CHECK_EQUAL( xml.attribute( "NumberType" ), "Float" );
  BOOST_CHECK_EQUAL( xml.attribute( "Precision" ), "4" );
  BOOST_CHECK_EQUAL( xml.attribute( "Format" ), "Dummy" );
  BOOST_CHECK_EQUAL( "DummyDatasetInitializationContent", xml.contentLine( 0 ) );
}

BOOST_AUTO_TEST_CASE( valueAtIndex ) {
  xdm::RefPtr< xdm::UniformDataItem > test = createData( 2, 2, 3 );
  int answer[] = {
     0,  1,
     2,  3, // end k = 0
     4,  5,
     6,  7, // end k = 1
     8,  9,
    10, 11
  };

  for ( int i = 0; i < 6; i++ ) {
    BOOST_CHECK_EQUAL( test->atIndex< int >( i ), answer[i] );
  }
}

BOOST_AUTO_TEST_CASE( valueAtLocation ) {
  int answer[2][2][3];
  int index = 0;
  for ( int i = 0; i < 2; ++i ) {
    for ( int j = 0; j < 2; ++j ) {
      for ( int k = 0; k < 3; ++k ) {
        answer[i][j][k] = index++;
      }
    }
  }

  xdm::RefPtr< xdm::UniformDataItem > item = createData( 2, 2, 3 );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 0, 0, 0 ), answer[0][0][0] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 0, 0, 1 ), answer[0][0][1] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 0, 0, 2 ), answer[0][0][2] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 0, 1, 0 ), answer[0][1][0] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 0, 1, 1 ), answer[0][1][1] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 0, 1, 2 ), answer[0][1][2] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 1, 0, 0 ), answer[1][0][0] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 1, 0, 1 ), answer[1][0][1] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 1, 0, 2 ), answer[1][0][2] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 1, 1, 0 ), answer[1][1][0] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 1, 1, 1 ), answer[1][1][1] );
  BOOST_CHECK_EQUAL( item->atLocation<int>( 1, 1, 2 ), answer[1][1][2] );
}

} // namespace
