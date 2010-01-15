#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/StructuredTopology.hpp>

TEST( StructuredTopology, writeMetadata ) {
  xdmGrid::StructuredTopology t;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  xdm::DataShape<> shape(3);
  shape[0] = 1;
  shape[1] = 2;
  shape[2] = 3;
  t.setShape( shape );

  t.writeMetadata( xml );

  ASSERT_EQ( "Topology", xml.tag() );
  ASSERT_EQ( "1 2 3", xml.attribute( "Dimensions" ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

