#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Attribute.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdm::RefPtr< xdmGrid::Attribute > test( 
    new xdmGrid::Attribute( xdmGrid::Attribute::kScalar, 
    xdmGrid::Attribute::kCell ) );
  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );

  test->writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Attribute", xml.tag() );
  BOOST_CHECK_EQUAL( "Scalar", xml.attribute( "AttributeType" ) );
  BOOST_CHECK_EQUAL( "Cell", xml.attribute( "Center" ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

