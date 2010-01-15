#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdm/CompositeDataItem.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
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

