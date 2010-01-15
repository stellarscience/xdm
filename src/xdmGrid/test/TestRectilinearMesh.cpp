#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/RectilinearMesh.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::RectilinearMesh t;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  xdm::DataShape<> ds(2);
  ds[0] = ds[1] = 2;
  t.setShape( ds );
  t.writeMetadata( xml );

  ASSERT_EQ( "Topology", xml.tag() );
  ASSERT_EQ( "2DRectMesh", xml.attribute( "TopologyType" ) );
  ASSERT_EQ( "2 2", xml.attribute( "Dimensions" ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

