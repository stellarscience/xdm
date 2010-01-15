#define BOOST_TEST_MODULE Polyvertex
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Polyvertex.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Polyvertex t;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  t.setNumberOfElements( 12 );
  t.setNodesPerElement( 0 );

  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Topology", xml.tag() );
  BOOST_CHECK_EQUAL( "Polyvertex", xml.attribute( "TopologyType" ) );
  BOOST_CHECK_EQUAL( "12", xml.attribute( "NumberOfElements" ) );
  BOOST_CHECK_EQUAL( "0", xml.attribute( "NodesPerElement" ) );
}

} // namespace 

