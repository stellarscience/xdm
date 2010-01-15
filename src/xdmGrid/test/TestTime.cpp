//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE Time 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Time.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Time t( 42.0 );
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Time", xml.tag() );
  BOOST_CHECK_EQUAL( "42", xml.attribute( "Value" ) );
}

} // namespace 

