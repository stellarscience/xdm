#define BOOST_TEST_MODULE RectilinearMesh 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/RectilinearMesh.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::RectilinearMesh t;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  xdm::DataShape<> ds(2);
  ds[0] = ds[1] = 2;
  t.setShape( ds );
  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Topology", xml.tag() );
  BOOST_CHECK_EQUAL( "2DRectMesh", xml.attribute( "TopologyType" ) );
  BOOST_CHECK_EQUAL( "2 2", xml.attribute( "Dimensions" ) );
}

} // namespace 
