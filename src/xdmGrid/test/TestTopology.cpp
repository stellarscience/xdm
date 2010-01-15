#define BOOST_TEST_MODULE Topology
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Topology.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Topology t;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Topology", xml.tag() );
}

