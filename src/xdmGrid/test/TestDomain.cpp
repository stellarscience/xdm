#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Domain.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::Domain d;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  d.writeMetadata( xml );

  ASSERT_EQ( "Domain", xml.tag() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

