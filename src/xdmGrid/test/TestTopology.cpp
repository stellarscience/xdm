//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE Topology
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Topology.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Topology t;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Topology", xml.tag() );
}

} // namespace 

