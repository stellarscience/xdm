#include <gtest/gtest.h>

#include <xdm/CompositeDataItem.hpp>

TEST( CompositeDataItem, writeMetadata ) {
  xdm::CompositeDataItem i;
  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml(obj);

  i.writeMetadata( xml );

  ASSERT_EQ( xml.tag(), "DataItem" );
  ASSERT_EQ( xml.attribute( "ItemType" ), "Tree" );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

