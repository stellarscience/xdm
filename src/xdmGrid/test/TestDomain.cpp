#include <gtest/gtest.h>

#include <xdmGrid/Domain.hpp>

TEST( Domain, writeMetadata ) {
  xdmGrid::Domain d;
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  d.writeMetadata( xml );

  ASSERT_EQ( "Domain", xml.tag() );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

