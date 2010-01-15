#define BOOST_TEST_MODULE CompositeDataItem 
#include <boost/test/unit_test.hpp>

#include <xdm/CompositeDataItem.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdm::CompositeDataItem i;
  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml(obj);

  i.writeMetadata( xml );

  BOOST_CHECK_EQUAL( xml.tag(), "DataItem" );
  BOOST_CHECK_EQUAL( xml.attribute( "ItemType" ), "Tree" );
}

