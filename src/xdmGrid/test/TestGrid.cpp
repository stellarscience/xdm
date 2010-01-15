#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Grid.hpp>

TEST( Grid, writeMetadata ) {
  xdmGrid::Grid g;
  g.setName( "Fred" );
  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );

  g.writeMetadata( xml );

  ASSERT_EQ( "Grid", xml.tag() );
  ASSERT_EQ( "Fred", xml.attribute( "Name" ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

