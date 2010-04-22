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
#define BOOST_TEST_MODULE ReferencingGrid
#include <boost/test/unit_test.hpp>

#include <xdmGrid/test/Cube.hpp>

#include <xdmGrid/Element.hpp>
#include <xdmGrid/InterlacedGeometry.hpp>
#include <xdmGrid/ReferencingGrid.hpp>
#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>
#include <xdmGrid/ElementTopology.hpp>

#include <xdm/test/TestHelpers.hpp>

#include <algorithm>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::ReferencingGrid g;

  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );
  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Grid", xml.tag() );
  BOOST_CHECK_EQUAL( "Referencing", xml.attribute( "GridType" ) );
}

BOOST_AUTO_TEST_CASE( indirectElementAccess ) {
  CubeOfTets cube;
  xdm::RefPtr< xdmGrid::InterlacedGeometry > g( new xdmGrid::InterlacedGeometry(3) );
  xdm::RefPtr< xdm::UniformDataItem > nodeList = test::createUniformDataItem(
    cube.nodeArray(), cube.numberOfNodes() * 3, xdm::primitiveType::kDouble );
  g->setCoordinateValues( nodeList );

  // There are 5 tetrahedra in the cube. This test will separate these into 3 topologies
  // as follows:
  //  1. Elements 0, 4
  //  2. Elements 3, 2
  //  4. Element 1
  //
  // For testing, we compare these to the values that come from the original cube.

  // Original cube
  xdm::RefPtr< xdmGrid::UnstructuredTopology > t0( new xdmGrid::UnstructuredTopology() );
  xdm::RefPtr< xdm::UniformDataItem > elementList0 = test::createUniformDataItem(
    cube.connectivityArray(), cube.numberOfElements() * 4, xdm::primitiveType::kLongUnsignedInt );
  t0->setConnectivity( elementList0 );
  t0->setNumberOfElements( 5 );
  t0->setElementTopology( xdmGrid::elementFactory( xdmGrid::ElementShape::Tetrahedron, 1 ) );
  xdm::RefPtr< xdmGrid::UniformGrid > grid0( new xdmGrid::UniformGrid );
  grid0->setGeometry( g );
  grid0->setTopology( t0 );

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
  grid1->setGeometry( g );
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
  grid2->setGeometry( g );
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
  grid3->setGeometry( g );
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

  xdmGrid::ReferencingGrid refGrid1;
  refGrid1.appendReferenceGrid( grid1, elements1Item );
  refGrid1.appendReferenceGrid( grid2, elements2Item );
  refGrid1.appendReferenceGrid( grid3, elements3Item );

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
  xdmGrid::ReferencingGrid refGrid2;
  refGrid2.appendReferenceGrid( grid1 );
  refGrid2.appendReferenceGrid( grid2 );
  refGrid2.appendReferenceGrid( grid3 );
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
  xdmGrid::ReferencingGrid refGrid3;
  refGrid3.appendReferenceGrid( grid0 );
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

} // namespace

