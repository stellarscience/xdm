#include <gtest/gtest.h>

#include <xdmGrid/UniformGrid.hpp>

TEST( UniformGrid, writeMetadata ) {
  xdmGrid::UniformGrid g;

  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );
  g.writeMetadata( xml );

  ASSERT_EQ( "Grid", xml.tag() );
  ASSERT_EQ( "Uniform", xml.attribute( "GridType" ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

