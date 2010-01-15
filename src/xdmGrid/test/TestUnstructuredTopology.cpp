#define BOOST_TEST_MODULE UnstructuredTopology
#include <boost/test/unit_test.hpp>

#include <xdmGrid/UnstructuredTopology.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::UnstructuredTopology t;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  t.setNumberOfElements( 42 );
  t.setNodesPerElement( 4 );

  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Topology", xml.tag() );
  BOOST_CHECK_EQUAL( "42", xml.attribute( "NumberOfElements" ) );
  BOOST_CHECK_EQUAL( "4", xml.attribute( "NodesPerElement" ) );
}
