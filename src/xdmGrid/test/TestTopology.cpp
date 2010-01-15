#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Topology.hpp>

TEST( Topology, writeMetadata ) {
  xdmGrid::Topology t;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  t.writeMetadata( xml );

  ASSERT_EQ( "Topology", xml.tag() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

