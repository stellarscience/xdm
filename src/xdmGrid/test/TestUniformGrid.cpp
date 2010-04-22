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
#define BOOST_TEST_MODULE UniformGrid
#include <boost/test/unit_test.hpp>

#include <xdmGrid/test/Cube.hpp>

#include <xdmGrid/Element.hpp>
#include <xdmGrid/ElementTopology.hpp>
#include <xdmGrid/InterlacedGeometry.hpp>
#include <xdmGrid/MultiArrayGeometry.hpp>
#include <xdmGrid/UniformGrid.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>

#include <xdm/test/TestHelpers.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::UniformGrid g;

  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );
  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Grid", xml.tag() );
  BOOST_CHECK_EQUAL( "Uniform", xml.attribute( "GridType" ) );
}

BOOST_AUTO_TEST_CASE( elementAccessSingleNodeArray ) {
  CubeOfTets cube;
  xdm::RefPtr< xdmGrid::InterlacedGeometry > g( new xdmGrid::InterlacedGeometry(3) );
  xdm::RefPtr< xdm::UniformDataItem > nodeList = test::createUniformDataItem(
    cube.nodeArray(), cube.numberOfNodes() * 3, xdm::primitiveType::kDouble );
  g->setCoordinateValues( nodeList );

  xdm::RefPtr< xdmGrid::UnstructuredTopology > t( new xdmGrid::UnstructuredTopology() );
  xdm::RefPtr< xdm::UniformDataItem > elementList = test::createUniformDataItem(
    cube.connectivityArray(), cube.numberOfElements() * 4, xdm::primitiveType::kLongUnsignedInt );
  t->setConnectivity( elementList );
  t->setNumberOfElements( 5 );
  t->setElementTopology( xdmGrid::elementFactory( xdmGrid::ElementShape::Tetrahedron, 1 ) );

  xdmGrid::UniformGrid grid;
  grid.setGeometry( g );
  grid.setTopology( t );

  // Check 2nd node of 2nd element.
  {
    xdmGrid::Element element = grid.element( 1 );
    BOOST_CHECK_EQUAL( 1., element.node( 1 )[0] );
    BOOST_CHECK_EQUAL( 0., element.node( 1 )[1] );
    BOOST_CHECK_EQUAL( 1., element.node( 1 )[2] );
  }

  // Change a node's location and check back in the geometry to see if it changed.
  {
    xdmGrid::Element element = grid.element( 3 );
    std::size_t nodeIndex = element.nodeIndexInGeometry( 2 );
    xdmGrid::Node node = g->node( nodeIndex );
    node[0] = 0.1;
    node[1] = 1.1;
    node[2] = 0.9;
    BOOST_CHECK_CLOSE( 0.1, g->node( 7 )[0], 1.e-8 );
    BOOST_CHECK_CLOSE( 1.1, g->node( 7 )[1], 1.e-8 );
    BOOST_CHECK_CLOSE( 0.9, g->node( 7 )[2], 1.e-8 );
  }
}

BOOST_AUTO_TEST_CASE( elementAccessMultiNodeArray ) {
  CubeOfTets cube;
  xdm::RefPtr< xdmGrid::MultiArrayGeometry > g( new xdmGrid::MultiArrayGeometry(3) );
  xdm::RefPtr< xdm::UniformDataItem > nodeListX = test::createUniformDataItem(
    cube.nodeX(), cube.numberOfNodes(), xdm::primitiveType::kDouble );
  xdm::RefPtr< xdm::UniformDataItem > nodeListY = test::createUniformDataItem(
    cube.nodeY(), cube.numberOfNodes(), xdm::primitiveType::kDouble );
  xdm::RefPtr< xdm::UniformDataItem > nodeListZ = test::createUniformDataItem(
    cube.nodeZ(), cube.numberOfNodes(), xdm::primitiveType::kDouble );
  g->setCoordinateValues( 0, nodeListX );
  g->setCoordinateValues( 1, nodeListY );
  g->setCoordinateValues( 2, nodeListZ );

  xdm::RefPtr< xdmGrid::UnstructuredTopology > t( new xdmGrid::UnstructuredTopology() );
  xdm::RefPtr< xdm::UniformDataItem > elementList = test::createUniformDataItem(
    cube.connectivityArray(), cube.numberOfElements() * 4, xdm::primitiveType::kLongUnsignedInt );
  t->setConnectivity( elementList );
  t->setNumberOfElements( 5 );
  t->setElementTopology( xdmGrid::elementFactory( xdmGrid::ElementShape::Tetrahedron, 1 ) );

  xdmGrid::UniformGrid grid;
  grid.setGeometry( g );
  grid.setTopology( t );

  // Check 2nd node of 2nd element.
  {
    xdmGrid::Element element = grid.element( 1 );
    BOOST_CHECK_EQUAL( 1., element.node( 1 )[0] );
    BOOST_CHECK_EQUAL( 0., element.node( 1 )[1] );
    BOOST_CHECK_EQUAL( 1., element.node( 1 )[2] );
  }

  // Change a node's location and check back in the geometry to see if it changed.
  {
    xdmGrid::Element element = grid.element( 3 );
    std::size_t nodeIndex = element.nodeIndexInGeometry( 2 );
    xdmGrid::Node node = g->node( nodeIndex );
    node[0] = 0.1;
    node[1] = 1.1;
    node[2] = 0.9;
    BOOST_CHECK_CLOSE( 0.1, g->node( 7 )[0], 1.e-8 );
    BOOST_CHECK_CLOSE( 1.1, g->node( 7 )[1], 1.e-8 );
    BOOST_CHECK_CLOSE( 0.9, g->node( 7 )[2], 1.e-8 );
  }
}

BOOST_AUTO_TEST_CASE( elementTopology ) {
  CubeOfTets cube;
  xdm::RefPtr< xdmGrid::InterlacedGeometry > g( new xdmGrid::InterlacedGeometry(3) );
  xdm::RefPtr< xdm::UniformDataItem > nodeList = test::createUniformDataItem(
    cube.nodeArray(), cube.numberOfNodes() * 3, xdm::primitiveType::kDouble );
  g->setCoordinateValues( nodeList );

  xdm::RefPtr< xdmGrid::UnstructuredTopology > t( new xdmGrid::UnstructuredTopology() );
  xdm::RefPtr< xdm::UniformDataItem > elementList = test::createUniformDataItem(
    cube.connectivityArray(), cube.numberOfElements() * 4, xdm::primitiveType::kLongUnsignedInt );
  t->setConnectivity( elementList );
  t->setNumberOfElements( 5 );
  t->setElementTopology( xdmGrid::elementFactory( xdmGrid::ElementShape::Tetrahedron, 1 ) );

  xdmGrid::UniformGrid grid;
  grid.setGeometry( g );
  grid.setTopology( t );

  {
    xdmGrid::Element element = grid.element( 1 );
    BOOST_CHECK_EQUAL( element.shape(), xdmGrid::ElementShape::Tetrahedron );
    BOOST_CHECK_EQUAL( element.numberOfFaces(), 4 );
    BOOST_CHECK_EQUAL( element.numberOfEdges(), 6 );
    BOOST_CHECK_EQUAL( element.numberOfNodes(), 4 );
    BOOST_CHECK_EQUAL( element.nodeIndexInGeometry( 0 ), 2 );
    BOOST_CHECK_EQUAL( element.nodeIndexInGeometry( 1 ), 5 );
    BOOST_CHECK_EQUAL( element.nodeIndexInGeometry( 2 ), 6 );
    BOOST_CHECK_EQUAL( element.nodeIndexInGeometry( 3 ), 7 );

    xdmGrid::Element face = element.face( 2 );
    BOOST_CHECK_EQUAL( face.shape(), xdmGrid::ElementShape::Triangle );
    BOOST_CHECK_EQUAL( face.numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( face.numberOfEdges(), 3 );
    BOOST_CHECK_EQUAL( face.numberOfNodes(), 3 );
    BOOST_CHECK_EQUAL( face.nodeIndexInGeometry( 0 ), 6 );
    BOOST_CHECK_EQUAL( face.nodeIndexInGeometry( 1 ), 2 );
    BOOST_CHECK_EQUAL( face.nodeIndexInGeometry( 2 ), 7 );

    xdmGrid::Element edge = element.edge( 4 );
    BOOST_CHECK_EQUAL( edge.shape(), xdmGrid::ElementShape::Curve );
    BOOST_CHECK_EQUAL( edge.numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge.numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge.numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge.nodeIndexInGeometry( 0 ), 5 );
    BOOST_CHECK_EQUAL( edge.nodeIndexInGeometry( 1 ), 7 );
  }
}

} // namespace
