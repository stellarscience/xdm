//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE Geometry 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Geometry.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Geometry g( 3 );
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Geometry", xml.tag() );
}

} // namespace 

