//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE Grid 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Grid.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Grid g;
  g.setName( "Fred" );
  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );

  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Grid", xml.tag() );
  BOOST_CHECK_EQUAL( "Fred", xml.attribute( "Name" ) );
}

} // namespace

