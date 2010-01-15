//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE Domain 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Domain.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Domain d;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  d.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Domain", xml.tag() );
}

} // namespace 

