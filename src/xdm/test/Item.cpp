#include <gtest/gtest.h>

#include <xdm/Item.hpp>

TEST( Item, writeMetadata ) {
  xdm::RefPtr< xdm::Item > i( new xdm::Item );
  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );
  i->writeMetadata( xml );

  char const * const answer = "Item";
  std::string result = xml.tag();
  ASSERT_EQ( answer, result );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

