#include <gtest/gtest.h>

#include <xdmGrid/TensorProductGeometry.hpp>

TEST( TensorProductGeometry, writeMetadata ) {
  xdmGrid::TensorProductGeometry g(3);
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );

  g.writeMetadata( xml );

  ASSERT_EQ( "Geometry", xml.tag() );
  ASSERT_EQ( "VxVyVz", xml.attribute( "GeometryType" ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

