//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
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
#define BOOST_TEST_MODULE UnstructuredTopology
#include <boost/test/unit_test.hpp>

#include <xdmGrid/test/Cube.hpp>

#include <xdmGrid/Cell.hpp>
#include <xdmGrid/InterlacedGeometry.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>

#include <xdm/test/TestHelpers.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::UnstructuredTopology t;
  xdm::XmlMetadataWrapper xml( xdm::makeRefPtr( new xdm::XmlObject ) );

  t.setNumberOfCells( 42 );
  t.setCellType( xdmGrid::CellType::Tetra );

  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Topology", xml.tag() );
  BOOST_CHECK_EQUAL( "42", xml.attribute( "NumberOfElements" ) );
  BOOST_CHECK_EQUAL( "4", xml.attribute( "NodesPerElement" ) );
}

BOOST_AUTO_TEST_CASE( cellAccess ) {
  CubeOfTets cube;
  xdm::RefPtr< xdmGrid::InterlacedGeometry > g( new xdmGrid::InterlacedGeometry(3) );
  xdm::RefPtr< xdm::UniformDataItem > nodeList = test::createUniformDataItem(
    cube.nodeArray(), cube.numberOfNodes() * 3, xdm::primitiveType::kDouble );
  g->setCoordinateValues( nodeList );

  xdmGrid::UnstructuredTopology t;
  xdm::RefPtr< xdm::UniformDataItem > cellList = test::createUniformDataItem(
    cube.connectivityArray(), cube.numberOfCells() * 4, xdm::primitiveType::kLongUnsignedInt );
  t.setConnectivity( cellList );
  t.setNumberOfCells( 5 );
  t.setCellType( xdmGrid::CellType::Tetra );
  t.setGeometry( g );

  // Check 2nd node of 2nd cell.
  xdmGrid::ConstCell cell1 = t.cell( 1 );
  BOOST_CHECK_EQUAL( 1., cell1.node( 1 )[0] );
  BOOST_CHECK_EQUAL( 0., cell1.node( 1 )[1] );
  BOOST_CHECK_EQUAL( 1., cell1.node( 1 )[2] );

  // Change a node's location and check back in the geometry to see if it changed.
  xdmGrid::Cell cell3 = t.cell( 3 );
  xdmGrid::Node node3 = cell3.node( 2 );
  node3[0] = 0.1;
  node3[1] = 1.1;
  node3[2] = 0.9;
  BOOST_CHECK_CLOSE( 0.1, g->node( 7 )[0], 1.e-8 );
  BOOST_CHECK_CLOSE( 1.1, g->node( 7 )[1], 1.e-8 );
  BOOST_CHECK_CLOSE( 0.9, g->node( 7 )[2], 1.e-8 );
}
