#include <gtest/gtest.h>

#include <xdmGrid/CollectionGrid.hpp>

struct CollectionGrid : ::testing::Test {
  xdm::RefPtr< xdmGrid::CollectionGrid > grid;
  xdm::XmlMetadataWrapper xml;
  CollectionGrid() : 
    grid( new xdmGrid::CollectionGrid ),
    xml( new xdm::XmlObject ) {}
};

TEST_F( CollectionGrid, writeMetadataSpatial ) {
  grid->setType( xdmGrid::CollectionGrid::kSpatial );
  grid->writeMetadata( xml );

  ASSERT_EQ( "Grid", xml.tag() );
  ASSERT_EQ( "Collection", xml.attribute( "GridType" ) );
  ASSERT_EQ( "Spatial", xml.attribute( "CollectionType" ) );
}

TEST_F( CollectionGrid, writeMetadataTemporal ) {
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

