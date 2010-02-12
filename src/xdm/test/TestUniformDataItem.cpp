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
#include <xdm/ArrayAdapter.hpp>

#include <algorithm>

namespace {

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

} // namespace
