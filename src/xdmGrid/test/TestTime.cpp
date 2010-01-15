#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Time.hpp>

TEST( Time, writeMetadata ) {
  xdmGrid::Time t( 42.0 );
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  t.writeMetadata( xml );

  ASSERT_EQ( "Time", xml.tag() );
  ASSERT_EQ( "42", xml.attribute( "Value" ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

