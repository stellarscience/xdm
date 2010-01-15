#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/CollectionGrid.hpp>

struct CollectionGrid : ::testing::Test {
  xdm::RefPtr< xdmGrid::CollectionGrid > grid;
  xdm::XmlMetadataWrapper xml;
  CollectionGrid() : 
    grid( new xdmGrid::CollectionGrid ),
    xml( new xdm::XmlObject ) {}
};

BOOST_AUTO_TEST_CASE( writeMetadataSpatial ) {
  grid->setType( xdmGrid::CollectionGrid::kSpatial );
  grid->writeMetadata( xml );

  ASSERT_EQ( "Grid", xml.tag() );
  ASSERT_EQ( "Collection", xml.attribute( "GridType" ) );
  ASSERT_EQ( "Spatial", xml.attribute( "CollectionType" ) );
}

BOOST_AUTO_TEST_CASE( writeMetadataTemporal ) {
  grid->setType( xdmGrid::CollectionGrid::kTemporal );
  grid->writeMetadata( xml );

  ASSERT_EQ( "Grid", xml.tag() );
  ASSERT_EQ( "Collection", xml.attribute( "GridType" ) );
  ASSERT_EQ( "Temporal", xml.attribute( "CollectionType" ) );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

