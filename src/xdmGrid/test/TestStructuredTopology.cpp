//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE StructuredTopology 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/StructuredTopology.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::StructuredTopology t;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  xdm::DataShape<> shape(3);
  shape[0] = 1;
  shape[1] = 2;
  shape[2] = 3;
  t.setShape( shape );

  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Topology", xml.tag() );
  BOOST_CHECK_EQUAL( "1 2 3", xml.attribute( "Dimensions" ) );
}

} // namespace 

