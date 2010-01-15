#define BOOST_TEST_MODULE UniformGrid 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/UniformGrid.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::UniformGrid g;

  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );
  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Grid", xml.tag() );
  BOOST_CHECK_EQUAL( "Uniform", xml.attribute( "GridType" ) );
}

} // namespace 

