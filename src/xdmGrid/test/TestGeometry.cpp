#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Geometry.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Geometry g( 3 );
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  g.writeMetadata( xml );

  ASSERT_EQ( "Geometry", xml.tag() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

