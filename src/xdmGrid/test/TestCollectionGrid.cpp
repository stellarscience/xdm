#define BOOST_TEST_MODULE CollectionGrid 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/CollectionGrid.hpp>

namespace {

struct Fixture {
  xdm::RefPtr< xdmGrid::CollectionGrid > grid;
  xdm::XmlMetadataWrapper xml;
  Fixture() : 
    grid( new xdmGrid::CollectionGrid ),
    xml( new xdm::XmlObject ) {}
};

BOOST_AUTO_TEST_CASE( writeMetadataSpatial ) {
  Fixture test;

  test.grid->setType( xdmGrid::CollectionGrid::kSpatial );
  test.grid->writeMetadata( test.xml );

  BOOST_CHECK_EQUAL( "Grid", test.xml.tag() );
  BOOST_CHECK_EQUAL( "Collection", test.xml.attribute( "GridType" ) );
  BOOST_CHECK_EQUAL( "Spatial", test.xml.attribute( "CollectionType" ) );
}

BOOST_AUTO_TEST_CASE( writeMetadataTemporal ) {
  Fixture test;

  test.grid->setType( xdmGrid::CollectionGrid::kTemporal );
  test.grid->writeMetadata( test.xml );

  BOOST_CHECK_EQUAL( "Grid", test.xml.tag() );
  BOOST_CHECK_EQUAL( "Collection", test.xml.attribute( "GridType" ) );
  BOOST_CHECK_EQUAL( "Temporal", test.xml.attribute( "CollectionType" ) );
}

} // namespace 

