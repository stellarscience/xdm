#include <gtest/gtest.h>

#include <xdmGrid/Geometry.hpp>

TEST( Geometry, writeMetadata ) {
  xdmGrid::Geometry g( 3 );
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  g.writeMetadata( xml );

  ASSERT_EQ( "Geometry", xml.tag() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

