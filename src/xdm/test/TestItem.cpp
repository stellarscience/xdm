#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/Item.hpp>

BOOST_AUTO_TEST_CASE( writeMetadataTag ) {
  xdm::RefPtr< xdm::Item > i( new xdm::Item );
  i->setName( "Fred" );

  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );
  i->writeMetadata( xml );

  char const * const tagAnswer = "Item";
  std::string tagResult = xml.tag();
  ASSERT_EQ( tagAnswer, tagResult );

  char const * const nameAnswer = "Fred";
  std::string nameResult = xml.attribute( "Name" );
  ASSERT_EQ( nameAnswer, nameResult );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

