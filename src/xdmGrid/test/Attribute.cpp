#include <gtest/gtest.h>

#include <xdmGrid/Attribute.hpp>

TEST( Attribute, writeMetadata ) {
  xdm::RefPtr< xdmGrid::Attribute > test( 
    new xdmGrid::Attribute( xdmGrid::Attribute::kScalar, 
    xdmGrid::Attribute::kCell ) );
  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );

  test->writeMetadata( xml );

  ASSERT_EQ( "Attribute", xml.tag() );
  ASSERT_EQ( "Scalar", xml.attribute( "AttributeType" ) );
  ASSERT_EQ( "Cell", xml.attribute( "Center" ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

