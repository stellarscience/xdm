#define BOOST_TEST_MODULE Geometry 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Geometry.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Geometry g( 3 );
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Geometry", xml.tag() );
}

