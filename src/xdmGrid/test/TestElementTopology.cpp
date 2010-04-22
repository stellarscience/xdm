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

#include <xdmGrid/ElementTopology.hpp>

namespace {

BOOST_AUTO_TEST_CASE( curve ) {

  {
    xdm::RefPtr< const xdmGrid::ElementTopology > curve =
      xdmGrid::elementFactory( xdmGrid::ElementShape::Curve, 1 );

    BOOST_CHECK_EQUAL( curve->name(), "Edge" );
    BOOST_CHECK_EQUAL( curve->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( curve->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( curve->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( curve->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( curve->node( 1 ), 1 );
  }

  {
    xdm::RefPtr< const xdmGrid::ElementTopology > curve =
      xdmGrid::elementFactory( xdmGrid::ElementShape::Curve, 2 );
    BOOST_CHECK_EQUAL( curve->name(), "Edge" );
    BOOST_CHECK_EQUAL( curve->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( curve->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( curve->numberOfNodes(), 3 );
    BOOST_CHECK_EQUAL( curve->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( curve->node( 1 ), 1 );
    BOOST_CHECK_EQUAL( curve->node( 2 ), 2 );
  }
}

BOOST_AUTO_TEST_CASE( triangle ) {

  xdm::RefPtr< const xdmGrid::ElementTopology > triangle =
    xdmGrid::elementFactory( xdmGrid::ElementShape::Triangle, 1 );

  BOOST_CHECK_EQUAL( triangle->name(), "Triangle" );
  BOOST_CHECK_EQUAL( triangle->numberOfFaces(), 0 );
  BOOST_CHECK_EQUAL( triangle->numberOfEdges(), 3 );
  BOOST_CHECK_EQUAL( triangle->numberOfNodes(), 3 );
  BOOST_CHECK_EQUAL( triangle->node( 0 ), 0 );
  BOOST_CHECK_EQUAL( triangle->node( 1 ), 1 );
  BOOST_CHECK_EQUAL( triangle->node( 2 ), 2 );

  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = triangle->edge( 0 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 1 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = triangle->edge( 1 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 2 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = triangle->edge( 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 0 );
  }
}

BOOST_AUTO_TEST_CASE( quad ) {

  xdm::RefPtr< const xdmGrid::ElementTopology > quad =
    xdmGrid::elementFactory( xdmGrid::ElementShape::Quadrilateral, 1 );

  BOOST_CHECK_EQUAL( quad->name(), "Quadrilateral" );
  BOOST_CHECK_EQUAL( quad->numberOfFaces(), 0 );
  BOOST_CHECK_EQUAL( quad->numberOfEdges(), 4 );
  BOOST_CHECK_EQUAL( quad->numberOfNodes(), 4 );
  BOOST_CHECK_EQUAL( quad->node( 0 ), 0 );
  BOOST_CHECK_EQUAL( quad->node( 1 ), 1 );
  BOOST_CHECK_EQUAL( quad->node( 2 ), 2 );
  BOOST_CHECK_EQUAL( quad->node( 3 ), 3 );

  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = quad->edge( 0 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 1 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = quad->edge( 1 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 2 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = quad->edge( 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = quad->edge( 3 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 3 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 0 );
  }
}

BOOST_AUTO_TEST_CASE( tetra ) {

  xdm::RefPtr< const xdmGrid::ElementTopology > tetra =
    xdmGrid::elementFactory( xdmGrid::ElementShape::Tetrahedron, 1 );

  BOOST_CHECK_EQUAL( tetra->name(), "Tetrahedron" );
  BOOST_CHECK_EQUAL( tetra->numberOfFaces(), 4 );
  BOOST_CHECK_EQUAL( tetra->numberOfEdges(), 6 );
  BOOST_CHECK_EQUAL( tetra->numberOfNodes(), 4 );
  BOOST_CHECK_EQUAL( tetra->node( 0 ), 0 );
  BOOST_CHECK_EQUAL( tetra->node( 1 ), 1 );
  BOOST_CHECK_EQUAL( tetra->node( 2 ), 2 );
  BOOST_CHECK_EQUAL( tetra->node( 3 ), 3 );

  // Edges
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = tetra->edge( 0 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 1 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = tetra->edge( 1 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 2 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = tetra->edge( 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 0 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = tetra->edge( 3 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = tetra->edge( 4 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = tetra->edge( 5 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 3 );
  }

  // Faces
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = tetra->face( 0 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 1 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = tetra->face( 1 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = tetra->face( 2 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = tetra->face( 3 );
    BOOST_CHECK_EQUAL( face->shape(), xdmGrid::ElementShape::Triangle );
    BOOST_CHECK_EQUAL( face->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( face->numberOfEdges(), 3 );
    BOOST_CHECK_EQUAL( face->numberOfNodes(), 3 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 1 );

    xdm::RefPtr< const xdmGrid::ElementTopology > edge = face->edge( 1 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 1 );
  }
}

BOOST_AUTO_TEST_CASE( pyramid ) {

  xdm::RefPtr< const xdmGrid::ElementTopology > pyramid =
    xdmGrid::elementFactory( xdmGrid::ElementShape::Pyramid, 1 );

  BOOST_CHECK_EQUAL( pyramid->name(), "Pyramid" );
  BOOST_CHECK_EQUAL( pyramid->numberOfFaces(), 5 );
  BOOST_CHECK_EQUAL( pyramid->numberOfEdges(), 8 );
  BOOST_CHECK_EQUAL( pyramid->numberOfNodes(), 5 );
  BOOST_CHECK_EQUAL( pyramid->node( 0 ), 0 );
  BOOST_CHECK_EQUAL( pyramid->node( 1 ), 1 );
  BOOST_CHECK_EQUAL( pyramid->node( 2 ), 2 );
  BOOST_CHECK_EQUAL( pyramid->node( 3 ), 3 );
  BOOST_CHECK_EQUAL( pyramid->node( 4 ), 4 );

  // Edges
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = pyramid->edge( 0 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 1 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = pyramid->edge( 1 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 2 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = pyramid->edge( 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = pyramid->edge( 3 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 3 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 0 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = pyramid->edge( 4 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = pyramid->edge( 5 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = pyramid->edge( 6 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = pyramid->edge( 7 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 3 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 4 );
  }

  // Triangular faces.
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = pyramid->face( 0 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 1 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = pyramid->face( 1 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = pyramid->face( 2 );
    BOOST_CHECK_EQUAL( face->shape(), xdmGrid::ElementShape::Triangle );
    BOOST_CHECK_EQUAL( face->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( face->numberOfEdges(), 3 );
    BOOST_CHECK_EQUAL( face->numberOfNodes(), 3 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 3 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 4 );

    xdm::RefPtr< const xdmGrid::ElementTopology > edge = face->edge( 2 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 4 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 2 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = pyramid->face( 3 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 3 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 4 );
  }

  // Check the quad on the bottom of the pyramid.
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = pyramid->face( 4 );
    BOOST_CHECK_EQUAL( face->shape(), xdmGrid::ElementShape::Quadrilateral );
    BOOST_CHECK_EQUAL( face->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( face->numberOfEdges(), 4 );
    BOOST_CHECK_EQUAL( face->numberOfNodes(), 4 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 3 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 1 );

    xdm::RefPtr< const xdmGrid::ElementTopology > edge = face->edge( 2 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 1 );
  }
}

BOOST_AUTO_TEST_CASE( wedge ) {

  xdm::RefPtr< const xdmGrid::ElementTopology > wedge =
    xdmGrid::elementFactory( xdmGrid::ElementShape::Wedge, 1 );

  BOOST_CHECK_EQUAL( wedge->name(), "Wedge" );
  BOOST_CHECK_EQUAL( wedge->numberOfFaces(), 5 );
  BOOST_CHECK_EQUAL( wedge->numberOfEdges(), 9 );
  BOOST_CHECK_EQUAL( wedge->numberOfNodes(), 6 );
  BOOST_CHECK_EQUAL( wedge->node( 0 ), 0 );
  BOOST_CHECK_EQUAL( wedge->node( 1 ), 1 );
  BOOST_CHECK_EQUAL( wedge->node( 2 ), 2 );
  BOOST_CHECK_EQUAL( wedge->node( 3 ), 3 );
  BOOST_CHECK_EQUAL( wedge->node( 4 ), 4 );
  BOOST_CHECK_EQUAL( wedge->node( 5 ), 5 );

  // Edges
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = wedge->edge( 0 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 1 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = wedge->edge( 1 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 2 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = wedge->edge( 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 0 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = wedge->edge( 3 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = wedge->edge( 4 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = wedge->edge( 5 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 5 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = wedge->edge( 6 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 3 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = wedge->edge( 7 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 4 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 5 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = wedge->edge( 8 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 5 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 3 );
  }

  // Quad faces.
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = wedge->face( 0 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 1 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 4 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = wedge->face( 1 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 5 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = wedge->face( 2 );
    BOOST_CHECK_EQUAL( face->shape(), xdmGrid::ElementShape::Quadrilateral );
    BOOST_CHECK_EQUAL( face->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( face->numberOfEdges(), 4 );
    BOOST_CHECK_EQUAL( face->numberOfNodes(), 4 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 3 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 5 );

    xdm::RefPtr< const xdmGrid::ElementTopology > edge = face->edge( 3 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 5 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 2 );
  }

  // Triangular faces.
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = wedge->face( 3 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 1 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = wedge->face( 4 );
    BOOST_CHECK_EQUAL( face->shape(), xdmGrid::ElementShape::Triangle );
    BOOST_CHECK_EQUAL( face->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( face->numberOfEdges(), 3 );
    BOOST_CHECK_EQUAL( face->numberOfNodes(), 3 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 3 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 4 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 5 );

    xdm::RefPtr< const xdmGrid::ElementTopology > edge = face->edge( 2 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 5 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 3 );
  }
}

BOOST_AUTO_TEST_CASE( hex ) {

  xdm::RefPtr< const xdmGrid::ElementTopology > hex =
    xdmGrid::elementFactory( xdmGrid::ElementShape::Hexahedron, 1 );

  BOOST_CHECK_EQUAL( hex->name(), "Hexahedron" );
  BOOST_CHECK_EQUAL( hex->numberOfFaces(), 6 );
  BOOST_CHECK_EQUAL( hex->numberOfEdges(), 12 );
  BOOST_CHECK_EQUAL( hex->numberOfNodes(), 8 );
  BOOST_CHECK_EQUAL( hex->node( 0 ), 0 );
  BOOST_CHECK_EQUAL( hex->node( 1 ), 1 );
  BOOST_CHECK_EQUAL( hex->node( 2 ), 2 );
  BOOST_CHECK_EQUAL( hex->node( 3 ), 3 );
  BOOST_CHECK_EQUAL( hex->node( 4 ), 4 );
  BOOST_CHECK_EQUAL( hex->node( 5 ), 5 );
  BOOST_CHECK_EQUAL( hex->node( 6 ), 6 );
  BOOST_CHECK_EQUAL( hex->node( 7 ), 7 );

  // Edges
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 0 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 1 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 1 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 2 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 3 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 3 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 3 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 0 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 4 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 5 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 5 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 6 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 6 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 7 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 3 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 7 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 8 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 4 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 5 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 9 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 5 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 6 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 10 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 6 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 7 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > edge = hex->edge( 11 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 7 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 4 );
  }

  // Faces
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = hex->face( 0 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 1 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 5 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 4 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = hex->face( 1 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 1 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 6 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 5 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = hex->face( 2 );
    BOOST_CHECK_EQUAL( face->shape(), xdmGrid::ElementShape::Quadrilateral );
    BOOST_CHECK_EQUAL( face->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( face->numberOfEdges(), 4 );
    BOOST_CHECK_EQUAL( face->numberOfNodes(), 4 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 3 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 7 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 6 );

    xdm::RefPtr< const xdmGrid::ElementTopology > edge = face->edge( 3 );
    BOOST_CHECK_EQUAL( edge->name(), "Edge" );
    BOOST_CHECK_EQUAL( edge->numberOfFaces(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfEdges(), 0 );
    BOOST_CHECK_EQUAL( edge->numberOfNodes(), 2 );
    BOOST_CHECK_EQUAL( edge->node( 0 ), 6 );
    BOOST_CHECK_EQUAL( edge->node( 1 ), 2 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = hex->face( 3 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 3 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 4 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 7 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = hex->face( 4 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 0 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 3 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 2 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 1 );
  }
  {
    xdm::RefPtr< const xdmGrid::ElementTopology > face = hex->face( 5 );
    BOOST_CHECK_EQUAL( face->node( 0 ), 4 );
    BOOST_CHECK_EQUAL( face->node( 1 ), 5 );
    BOOST_CHECK_EQUAL( face->node( 2 ), 6 );
    BOOST_CHECK_EQUAL( face->node( 3 ), 7 );
  }
}

} // namespace
