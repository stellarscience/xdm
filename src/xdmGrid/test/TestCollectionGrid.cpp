//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009-2010 Stellar Science. Government-purpose rights granted.
//
// This file is part of XDM
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------------------------------------------------------
#define BOOST_TEST_MODULE CollectionGrid
#include <boost/test/unit_test.hpp>

#include <xdmGrid/CollectionGrid.hpp>
#include <xdmGrid/Element.hpp>
#include <xdmGrid/ElementTopology.hpp>
#include <xdmGrid/InterlacedGeometry.hpp>
#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>

#include <xdmGrid/test/Cube.hpp>

#include <xdm/test/TestHelpers.hpp>

#include <algorithm>

namespace {

struct Fixture {
  xdm::RefPtr< xdmGrid::CollectionGrid > grid;
  xdm::XmlMetadataWrapper xml;
  Fixture() :
    grid( new xdmGrid::CollectionGrid ),
    xml( xdm::makeRefPtr( new xdm::XmlObject ) ) {}
};

xdm::RefPtr< xdmGrid::UniformGrid > cubeGrid() {
  CubeOfTets cube;

  // Geometry
  xdm::RefPtr< xdmGrid::InterlacedGeometry > g( new xdmGrid::InterlacedGeometry(3) );
  xdm::RefPtr< xdm::UniformDataItem > nodeList = test::createUniformDataItem(
    cube.nodeArray(), cube.numberOfNodes() * 3, xdm::primitiveType::kDouble );
  g->setCoordinateValues( nodeList );

  // Topology
  xdm::RefPtr< xdmGrid::UnstructuredTopology > t0( new xdmGrid::UnstructuredTopology() );
  xdm::RefPtr< xdm::UniformDataItem > elementList0 = test::createUniformDataItem(
    cube.connectivityArray(), cube.numberOfElements() * 4, xdm::primitiveType::kLongUnsignedInt );
  t0->setConnectivity( elementList0 );
  t0->setNumberOfElements( 5 );
  t0->setElementTopology( xdmGrid::elementFactory( xdmGrid::ElementShape::Tetrahedron, 1 ) );
  xdm::RefPtr< xdmGrid::UniformGrid > grid( new xdmGrid::UniformGrid );
  grid->setGeometry( g );
  grid->setTopology( t0 );
  return grid;
}

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

BOOST_AUTO_TEST_CASE( indirectElementAccess ) {

  // There are 5 tetrahedra in the cube. This test will separate these into 3 topologies
  // as follows:
  //  1. Elements 0, 4
  //  2. Elements 3, 2
  //  4. Element 1
  //
  // For testing, we compare these to the values that come from the original cube.

  xdm::RefPtr< xdmGrid::UniformGrid > grid0 = cubeGrid();
  CubeOfTets cube;

  // Topology 1
  xdm::RefPtr< xdmGrid::UnstructuredTopology > t1( new xdmGrid::UnstructuredTopology() );
  std::size_t connectivity1[ 8 ];
  std::copy( cube.connectivityArray(), cube.connectivityArray() + 4, connectivity1 );
  std::copy( cube.connectivityArray() + 16, cube.connectivityArray() + 20, connectivity1 + 4 );
  xdm::RefPtr< xdm::UniformDataItem > elementList1 = test::createUniformDataItem(
    connectivity1, 8, xdm::primitiveType::kLongUnsignedInt );
  t1->setConnectivity( elementList1 );
  t1->setNumberOfElements( 2 );
  t1->setElementTopology( xdmGrid::elementFactory( xdmGrid::ElementShape::Tetrahedron, 1 ) );
  xdm::RefPtr< xdmGrid::UniformGrid > grid1( new xdmGrid::UniformGrid );
  grid1->setGeometry( grid0->geometry() );
  grid1->setTopology( t1 );

  // Topology 2
  xdm::RefPtr< xdmGrid::UnstructuredTopology > t2( new xdmGrid::UnstructuredTopology() );
  std::size_t connectivity2[ 8 ];
  std::copy( cube.connectivityArray() + 12, cube.connectivityArray() + 16, connectivity2 );
  std::copy( cube.connectivityArray() + 8, cube.connectivityArray() + 12, connectivity2 + 4 );
  xdm::RefPtr< xdm::UniformDataItem > elementList2 = test::createUniformDataItem(
    connectivity2, 8, xdm::primitiveType::kLongUnsignedInt );
  t2->setConnectivity( elementList2 );
  t2->setNumberOfElements( 2 );
  t2->setElementTopology( xdmGrid::elementFactory( xdmGrid::ElementShape::Tetrahedron, 1 ) );
  xdm::RefPtr< xdmGrid::UniformGrid > grid2( new xdmGrid::UniformGrid );
  grid2->setGeometry( grid0->geometry() );
  grid2->setTopology( t2 );

  // Topology 3
  xdm::RefPtr< xdmGrid::UnstructuredTopology > t3( new xdmGrid::UnstructuredTopology() );
  std::size_t connectivity3[ 4 ];
  std::copy( cube.connectivityArray() + 4, cube.connectivityArray() + 8, connectivity3 );
  xdm::RefPtr< xdm::UniformDataItem > elementList3 = test::createUniformDataItem(
    connectivity3, 4, xdm::primitiveType::kLongUnsignedInt );
  t3->setConnectivity( elementList3 );
  t3->setNumberOfElements( 1 );
  t3->setElementTopology( xdmGrid::elementFactory( xdmGrid::ElementShape::Tetrahedron, 1 ) );
  xdm::RefPtr< xdmGrid::UniformGrid > grid3( new xdmGrid::UniformGrid );
  grid3->setGeometry( grid0->geometry() );
  grid3->setTopology( t3 );

  // The referenced grid takes data items that list the element indices. We just take them in
  // order.
  std::size_t elementOrdering[] = { 0, 1 };
  xdm::RefPtr< xdm::UniformDataItem > elements1Item = test::createUniformDataItem(
    elementOrdering, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > elements2Item = test::createUniformDataItem(
    elementOrdering, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > elements3Item = test::createUniformDataItem(
    elementOrdering, 1, xdm::primitiveType::kLongUnsignedInt );

  xdmGrid::CollectionGrid refGrid1;
  refGrid1.appendGrid( grid1, elements1Item );
  refGrid1.appendGrid( grid2, elements2Item );
  refGrid1.appendGrid( grid3, elements3Item );

  // Check to see if the node values from the referenced grid match the original
  // nodes.
  std::size_t elementMap[] = { 0, 4, 3, 2, 1 };
  BOOST_CHECK_EQUAL( refGrid1.numberOfElements(), 5 );
  for ( std::size_t elementIndex = 0; elementIndex < 5; ++elementIndex ) {
    xdmGrid::Element elementOrig = grid0->element( elementMap[ elementIndex ] );
    xdmGrid::Element elementRef = refGrid1.element( elementIndex );
    for ( std::size_t nodeIndex = 0; nodeIndex < 4; ++nodeIndex ) {
      for ( std::size_t dim = 0; dim < 3; ++dim ) {
        double refValue = elementRef.node( nodeIndex )[ dim ];
        double origValue = elementOrig.node( nodeIndex )[ dim ];
        BOOST_CHECK_EQUAL( origValue, refValue );
      }
    }
  }

  // Test again, this time without providing the data items. Because elementOrdering is just
  // default ordering, this new grid should reference the original grids in exactly the
  // same way.
  xdmGrid::CollectionGrid refGrid2;
  refGrid2.appendGrid( grid1 );
  refGrid2.appendGrid( grid2 );
  refGrid2.appendGrid( grid3 );
  BOOST_CHECK_EQUAL( refGrid2.numberOfElements(), 5 );
  for ( std::size_t elementIndex = 0; elementIndex < 5; ++elementIndex ) {
    xdmGrid::Element elementOrig = grid0->element( elementMap[ elementIndex ] );
    xdmGrid::Element elementRef = refGrid2.element( elementIndex );
    for ( std::size_t nodeIndex = 0; nodeIndex < 4; ++nodeIndex ) {
      for ( std::size_t dim = 0; dim < 3; ++dim ) {
        double refValue = elementRef.node( nodeIndex )[ dim ];
        double origValue = elementOrig.node( nodeIndex )[ dim ];
        BOOST_CHECK_EQUAL( origValue, refValue );
      }
    }
  }

  // May as well also test it with just one grid.
  xdmGrid::CollectionGrid refGrid3;
  refGrid3.appendGrid( grid0 );
  BOOST_CHECK_EQUAL( refGrid3.numberOfElements(), 5 );
  for ( std::size_t elementIndex = 0; elementIndex < 5; ++elementIndex ) {
    xdmGrid::Element elementOrig = grid0->element( elementIndex );
    xdmGrid::Element elementRef = refGrid3.element( elementIndex );
    for ( std::size_t nodeIndex = 0; nodeIndex < 4; ++nodeIndex ) {
      for ( std::size_t dim = 0; dim < 3; ++dim ) {
        double refValue = elementRef.node( nodeIndex )[ dim ];
        double origValue = elementOrig.node( nodeIndex )[ dim ];
        BOOST_CHECK_EQUAL( origValue, refValue );
      }
    }
  }

}

BOOST_AUTO_TEST_CASE( faceAccess ) {

  // A smaller, easier version of the above test. Added face access testing.

  // There are 5 tetrahedra in the cube. This test will separate these into 3 topologies
  // as follows:
  //  1. Elements 0, 4
  //  2. Elements 3, 2
  //  4. Element 1
  //
  // For testing, we compare these to the values that come from the original cube.

  xdm::RefPtr< xdmGrid::UniformGrid > grid0 = cubeGrid();

  // The referenced grid takes data items that list the element indices. We just take them in
  // order.
  std::size_t elementOrdering1[] = { 0, 4 };
  std::size_t elementOrdering2[] = { 3, 2 };
  std::size_t elementOrdering3[] = { 1 };
  xdm::RefPtr< xdm::UniformDataItem > elements1Item = test::createUniformDataItem(
    elementOrdering1, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > elements2Item = test::createUniformDataItem(
    elementOrdering2, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > elements3Item = test::createUniformDataItem(
    elementOrdering3, 1, xdm::primitiveType::kLongUnsignedInt );

  // Reference some faces on the elements in some arbitrary order.
  std::size_t faceOrdering1[] = { 3, 2 };
  std::size_t faceOrdering2[] = { 2, 1 };
  std::size_t faceOrdering3[] = { 0 };
  xdm::RefPtr< xdm::UniformDataItem > faces1 = test::createUniformDataItem(
    faceOrdering1, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > faces2 = test::createUniformDataItem(
    faceOrdering2, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > faces3 = test::createUniformDataItem(
    faceOrdering3, 1, xdm::primitiveType::kLongUnsignedInt );

  xdmGrid::CollectionGrid refGrid;
  refGrid.appendGridFaces( grid0, elements1Item, faces1 );
  refGrid.appendGridFaces( grid0, elements2Item, faces2 );
  refGrid.appendGridFaces( grid0, elements3Item, faces3 );

  // Check to see if the node values from the referenced grid match the original
  // nodes.
  std::size_t elementMap[] = { 0, 4, 3, 2, 1 };
  std::size_t faceMap[] = { 3, 2, 2, 1, 0 };
  BOOST_CHECK_EQUAL( refGrid.numberOfElements(), 5 );
  for ( std::size_t elementIndex = 0; elementIndex < 5; ++elementIndex ) {
    xdmGrid::Element faceOrig =
      grid0->element( elementMap[ elementIndex ] ).face( faceMap[ elementIndex ] );
    xdmGrid::Element faceRef = refGrid.element( elementIndex );
    BOOST_REQUIRE_EQUAL( faceRef.numberOfNodes(), 3 );
    BOOST_REQUIRE_EQUAL( faceOrig.numberOfNodes(), 3 );
    BOOST_REQUIRE_EQUAL( faceRef.shape(), xdmGrid::ElementShape::Triangle );
    BOOST_REQUIRE_EQUAL( faceOrig.shape(), xdmGrid::ElementShape::Triangle );
    for ( std::size_t nodeIndex = 0; nodeIndex < 3; ++nodeIndex ) {
      for ( std::size_t dim = 0; dim < 3; ++dim ) {
        double refValue = faceRef.node( nodeIndex )[ dim ];
        double origValue = faceOrig.node( nodeIndex )[ dim ];
        BOOST_CHECK_EQUAL( origValue, refValue );
      }
    }
  }
}

BOOST_AUTO_TEST_CASE( edgeAccess ) {

  // A smaller, easier version of the above test. Added edge access testing.

  // There are 5 tetrahedra in the cube. This test will separate these into 3 topologies
  // as follows:
  //  1. Elements 0, 4
  //  2. Elements 3, 2
  //  4. Element 1
  //
  // For testing, we compare these to the values that come from the original cube.

  xdm::RefPtr< xdmGrid::UniformGrid > grid0 = cubeGrid();

  // The referenced grid takes data items that list the element indices. We just take them in
  // order.
  std::size_t elementOrdering1[] = { 0, 4 };
  std::size_t elementOrdering2[] = { 3, 2 };
  std::size_t elementOrdering3[] = { 1 };
  xdm::RefPtr< xdm::UniformDataItem > elements1Item = test::createUniformDataItem(
    elementOrdering1, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > elements2Item = test::createUniformDataItem(
    elementOrdering2, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > elements3Item = test::createUniformDataItem(
    elementOrdering3, 1, xdm::primitiveType::kLongUnsignedInt );

  // Reference some edges on the elements in some arbitrary order.
  std::size_t edgeOrdering1[] = { 5, 3 };
  std::size_t edgeOrdering2[] = { 4, 1 };
  std::size_t edgeOrdering3[] = { 0 };
  xdm::RefPtr< xdm::UniformDataItem > edges1 = test::createUniformDataItem(
    edgeOrdering1, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > edges2 = test::createUniformDataItem(
    edgeOrdering2, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > edges3 = test::createUniformDataItem(
    edgeOrdering3, 1, xdm::primitiveType::kLongUnsignedInt );

  xdmGrid::CollectionGrid refGrid;
  refGrid.appendGridEdges( grid0, elements1Item, edges1 );
  refGrid.appendGridEdges( grid0, elements2Item, edges2 );
  refGrid.appendGridEdges( grid0, elements3Item, edges3 );

  // Check to see if the node values from the referenced grid match the original
  // nodes.
  std::size_t elementMap[] = { 0, 4, 3, 2, 1 };
  std::size_t edgeMap[] = { 5, 3, 4, 1, 0 };
  BOOST_CHECK_EQUAL( refGrid.numberOfElements(), 5 );
  for ( std::size_t elementIndex = 0; elementIndex < 5; ++elementIndex ) {
    xdmGrid::Element edgeOrig =
      grid0->element( elementMap[ elementIndex ] ).edge( edgeMap[ elementIndex ] );
    xdmGrid::Element edgeRef = refGrid.element( elementIndex );
    BOOST_REQUIRE_EQUAL( edgeRef.numberOfNodes(), 2 );
    BOOST_REQUIRE_EQUAL( edgeOrig.numberOfNodes(), 2 );
    BOOST_REQUIRE_EQUAL( edgeRef.shape(), xdmGrid::ElementShape::Curve );
    BOOST_REQUIRE_EQUAL( edgeOrig.shape(), xdmGrid::ElementShape::Curve );
    for ( std::size_t nodeIndex = 0; nodeIndex < 3; ++nodeIndex ) {
      for ( std::size_t dim = 0; dim < 3; ++dim ) {
        double refValue = edgeRef.node( nodeIndex )[ dim ];
        double origValue = edgeOrig.node( nodeIndex )[ dim ];
        BOOST_CHECK_EQUAL( origValue, refValue );
      }
    }
  }
}

} // namespace

