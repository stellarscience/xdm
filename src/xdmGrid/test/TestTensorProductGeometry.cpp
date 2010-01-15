//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#define BOOST_TEST_MODULE TensorProductGeometry 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/TensorProductGeometry.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::TensorProductGeometry g(3);
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Geometry", xml.tag() );
  BOOST_CHECK_EQUAL( "VxVyVz", xml.attribute( "GeometryType" ) );
}

} // namespace 

