#define BOOST_TEST_MODULE TensorProductGeometry 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/InterlacedGeometry.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::InterlacedGeometry g(3);
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Geometry", xml.tag() );
  BOOST_CHECK_EQUAL( "XYZ", xml.attribute( "GeometryType" ) );
}

} // namespace 

