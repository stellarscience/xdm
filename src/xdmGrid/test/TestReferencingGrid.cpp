//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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

#include <xdmGrid/Cell.hpp>
#include <xdmGrid/InterlacedGeometry.hpp>
#include <xdmGrid/ReferencingGrid.hpp>
#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>

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

BOOST_AUTO_TEST_CASE( indirectCellAccess ) {
  CubeOfTets cube;
  xdm::RefPtr< xdmGrid::InterlacedGeometry > g( new xdmGrid::InterlacedGeometry(3) );
  xdm::RefPtr< xdm::UniformDataItem > nodeList = test::createUniformDataItem(
    cube.nodeArray(), cube.numberOfNodes() * 3, xdm::primitiveType::kDouble );
  g->setCoordinateValues( nodeList );

  // There are 5 tetrahedra in the cube. This test will separate these into 3 topologies
  // as follows:
  //  1. Cells 0, 4
  //  2. Cells 3, 2
  //  4. Cell 1
  //
  // For testing, we compare these to the values that come from the original cube.

  // Original cube
  xdm::RefPtr< xdmGrid::UnstructuredTopology > t0( new xdmGrid::UnstructuredTopology() );
  xdm::RefPtr< xdm::UniformDataItem > cellList0 = test::createUniformDataItem(
    cube.connectivityArray(), cube.numberOfCells() * 4, xdm::primitiveType::kLongUnsignedInt );
  t0->setConnectivity( cellList0 );
  t0->setNumberOfCells( 5 );
  t0->setCellType( xdmGrid::CellType::Tetra );
  xdm::RefPtr< xdmGrid::UniformGrid > grid0( new xdmGrid::UniformGrid );
  grid0->setGeometry( g );
  grid0->setTopology( t0 );

  // Topology 1
  xdm::RefPtr< xdmGrid::UnstructuredTopology > t1( new xdmGrid::UnstructuredTopology() );
  std::size_t connectivity1[ 8 ];
  std::copy( cube.connectivityArray(), cube.connectivityArray() + 4, connectivity1 );
  std::copy( cube.connectivityArray() + 16, cube.connectivityArray() + 20, connectivity1 + 4 );
  xdm::RefPtr< xdm::UniformDataItem > cellList1 = test::createUniformDataItem(
    connectivity1, 8, xdm::primitiveType::kLongUnsignedInt );
  t1->setConnectivity( cellList1 );
  t1->setNumberOfCells( 2 );
  t1->setCellType( xdmGrid::CellType::Tetra );
  xdm::RefPtr< xdmGrid::UniformGrid > grid1( new xdmGrid::UniformGrid );
  grid1->setGeometry( g );
  grid1->setTopology( t1 );

  // Topology 2
  xdm::RefPtr< xdmGrid::UnstructuredTopology > t2( new xdmGrid::UnstructuredTopology() );
  std::size_t connectivity2[ 8 ];
  std::copy( cube.connectivityArray() + 12, cube.connectivityArray() + 16, connectivity2 );
  std::copy( cube.connectivityArray() + 8, cube.connectivityArray() + 12, connectivity2 + 4 );
  xdm::RefPtr< xdm::UniformDataItem > cellList2 = test::createUniformDataItem(
    connectivity2, 8, xdm::primitiveType::kLongUnsignedInt );
  t2->setConnectivity( cellList2 );
  t2->setNumberOfCells( 2 );
  t2->setCellType( xdmGrid::CellType::Tetra );
  xdm::RefPtr< xdmGrid::UniformGrid > grid2( new xdmGrid::UniformGrid );
  grid2->setGeometry( g );
  grid2->setTopology( t2 );

  // Topology 3
  xdm::RefPtr< xdmGrid::UnstructuredTopology > t3( new xdmGrid::UnstructuredTopology() );
  std::size_t connectivity3[ 4 ];
  std::copy( cube.connectivityArray() + 4, cube.connectivityArray() + 8, connectivity3 );
  xdm::RefPtr< xdm::UniformDataItem > cellList3 = test::createUniformDataItem(
    connectivity3, 4, xdm::primitiveType::kLongUnsignedInt );
  t3->setConnectivity( cellList3 );
  t3->setNumberOfCells( 1 );
  t3->setCellType( xdmGrid::CellType::Tetra );
  xdm::RefPtr< xdmGrid::UniformGrid > grid3( new xdmGrid::UniformGrid );
  grid3->setGeometry( g );
  grid3->setTopology( t3 );

  // The referenced grid takes data items that list the cell indices. We just take them in
  // order.
  std::size_t cellOrdering[] = { 0, 1 };
  xdm::RefPtr< xdm::UniformDataItem > cells1Item = test::createUniformDataItem(
    cellOrdering, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > cells2Item = test::createUniformDataItem(
    cellOrdering, 2, xdm::primitiveType::kLongUnsignedInt );
  xdm::RefPtr< xdm::UniformDataItem > cells3Item = test::createUniformDataItem(
    cellOrdering, 1, xdm::primitiveType::kLongUnsignedInt );

  xdmGrid::ReferencingGrid refGrid1;
  refGrid1.appendReferenceGrid( grid1, cells1Item );
  refGrid1.appendReferenceGrid( grid2, cells2Item );
  refGrid1.appendReferenceGrid( grid3, cells3Item );

  // Check to see if the node values from the referenced grid match the original
  // nodes.
  std::size_t cellMap[] = { 0, 4, 3, 2, 1 };
  BOOST_CHECK_EQUAL( refGrid1.numberOfCells(), 5 );
  for ( std::size_t cellIndex = 0; cellIndex < 5; ++cellIndex ) {
    xdmGrid::ConstCell cellOrig = grid0->cell( cellMap[ cellIndex ] );
    xdmGrid::ConstCell cellRef = refGrid1.cell( cellIndex );
    for ( std::size_t nodeIndex = 0; nodeIndex < 4; ++nodeIndex ) {
      for ( std::size_t dim = 0; dim < 3; ++dim ) {
        double refValue = cellRef.node( nodeIndex )[ dim ];
        double origValue = cellOrig.node( nodeIndex )[ dim ];
        BOOST_CHECK_EQUAL( origValue, refValue );
      }
    }
  }

  // Test again, this time without providing the data items. Because cellOrdering is just
  // default ordering, this new grid should reference the original grids in exactly the
  // same way.
  xdmGrid::ReferencingGrid refGrid2;
  refGrid2.appendReferenceGrid( grid1 );
  refGrid2.appendReferenceGrid( grid2 );
  refGrid2.appendReferenceGrid( grid3 );
  BOOST_CHECK_EQUAL( refGrid2.numberOfCells(), 5 );
  for ( std::size_t cellIndex = 0; cellIndex < 5; ++cellIndex ) {
    xdmGrid::ConstCell cellOrig = grid0->cell( cellMap[ cellIndex ] );
    xdmGrid::ConstCell cellRef = refGrid2.cell( cellIndex );
    for ( std::size_t nodeIndex = 0; nodeIndex < 4; ++nodeIndex ) {
      for ( std::size_t dim = 0; dim < 3; ++dim ) {
        double refValue = cellRef.node( nodeIndex )[ dim ];
        double origValue = cellOrig.node( nodeIndex )[ dim ];
        BOOST_CHECK_EQUAL( origValue, refValue );
      }
    }
  }

  // May as well also test it with just one grid.
  xdmGrid::ReferencingGrid refGrid3;
  refGrid3.appendReferenceGrid( grid0 );
  BOOST_CHECK_EQUAL( refGrid3.numberOfCells(), 5 );
  for ( std::size_t cellIndex = 0; cellIndex < 5; ++cellIndex ) {
    xdmGrid::ConstCell cellOrig = grid0->cell( cellIndex );
    xdmGrid::ConstCell cellRef = refGrid3.cell( cellIndex );
    for ( std::size_t nodeIndex = 0; nodeIndex < 4; ++nodeIndex ) {
      for ( std::size_t dim = 0; dim < 3; ++dim ) {
        double refValue = cellRef.node( nodeIndex )[ dim ];
        double origValue = cellOrig.node( nodeIndex )[ dim ];
        BOOST_CHECK_EQUAL( origValue, refValue );
      }
    }
  }

}

} // namespace

