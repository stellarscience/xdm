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
#include <xdmGrid/ElementTopology.hpp>

#include <cassert>
#include <map>
#include <sstream>
#include <stdexcept>

XDM_GRID_NAMESPACE_BEGIN

namespace {

template< typename T1, typename T2 >
void fill( std::vector< T1 >& v, T2 a, T2 b ) { v[0] = a; v[1] = b; }

template< typename T1, typename T2 >
void fill( std::vector< T1 >& v, T2 a, T2 b, T2 c ) { v[0] = a; v[1] = b; v[2] = c; }

template< typename T1, typename T2 >
void fill( std::vector< T1 >& v, T2 a, T2 b, T2 c, T2 d ) { v[0] = a; v[1] = b; v[2] = c; v[3] = d; }

template< typename T1, typename T2 >
void fill( std::vector< T1 >& v, T2 a, T2 b, T2 c, T2 d, T2 e ) {
  v[0] = a; v[1] = b; v[2] = c; v[3] = d; v[4] = e;
}

template< typename T1, typename T2 >
void fill( std::vector< T1 >& v, T2 a, T2 b, T2 c, T2 d, T2 e, T2 f ) {
  v[0] = a; v[1] = b; v[2] = c; v[3] = d; v[4] = e; v[5] = f;
}

template< typename T1, typename T2 >
void fill( std::vector< T1 >& v, T2 a, T2 b, T2 c, T2 d, T2 e, T2 f, T2 g ) {
  v[0] = a; v[1] = b; v[2] = c; v[3] = d; v[4] = e; v[5] = f; v[6] = g;
}

template< typename T1, typename T2 >
void fill( std::vector< T1 >& v, T2 a, T2 b, T2 c, T2 d, T2 e, T2 f, T2 g, T2 h ) {
  v[0] = a; v[1] = b; v[2] = c; v[3] = d; v[4] = e; v[5] = f; v[6] = g; v[7] = h;
}

template< typename T1, typename T2 >
void fill( std::vector< T1 >& v, T2 a, T2 b, T2 c, T2 d, T2 e, T2 f, T2 g, T2 h, T2 i ) {
  v[0] = a; v[1] = b; v[2] = c; v[3] = d; v[4] = e; v[5] = f; v[6] = g; v[7] = h; v[8] = i;
}

template <typename ForwardIterator, typename T>
void iota( ForwardIterator first, ForwardIterator last, T value ) {
  for ( ; first != last; ++first, ++value ) {
    *first = value;
  }
}

// Linear edge element with just a begin and end point.
xdm::RefPtr< const ElementTopology > edgeWithNodes( std::size_t n0, std::size_t n1 ) {
  std::vector< xdm::RefPtr< const ElementTopology > > empty;
  std::vector< std::size_t > edgeNodes( 2 );
  fill( edgeNodes, n0, n1 );
  return xdm::makeRefPtr(
    new ElementTopology( empty, empty, edgeNodes, ElementShape::Curve, "Edge" ) );
}

// Quadratic edge element with begin, end, and midpoint.
xdm::RefPtr< const ElementTopology > edgeWithNodes( std::size_t n0, std::size_t n1, std::size_t n2 ) {
  std::vector< xdm::RefPtr< const ElementTopology > > empty;
  std::vector< std::size_t > edgeNodes( 3 );
  fill( edgeNodes, n0, n1, n2 );
  return xdm::makeRefPtr(
    new ElementTopology( empty, empty, edgeNodes, ElementShape::Curve, "Edge" ) );
}

// Linear triangle element with 3 vertices.
xdm::RefPtr< const ElementTopology > triWithNodes( std::size_t n0, std::size_t n1, std::size_t n2 ) {
  std::vector< xdm::RefPtr< const ElementTopology > > empty;
  std::vector< std::size_t > nodes( 3 );
  fill( nodes, n0, n1, n2 );
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  edges.push_back( edgeWithNodes( n0, n1 ) );
  edges.push_back( edgeWithNodes( n1, n2 ) );
  edges.push_back( edgeWithNodes( n2, n0 ) );
  return xdm::makeRefPtr(
    new ElementTopology( empty, edges, nodes, ElementShape::Triangle, "Triangle" ) );
}

// Quadratic triangle element with 3 vertices and 3 midpoints.
xdm::RefPtr< const ElementTopology > triWithNodes( std::size_t n0, std::size_t n1, std::size_t n2,
  std::size_t n3, std::size_t n4, std::size_t n5 ) {

  std::vector< xdm::RefPtr< const ElementTopology > > empty;
  std::vector< std::size_t > nodes( 6 );
  fill( nodes, n0, n1, n2, n3, n4, n5 );
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  edges.push_back( edgeWithNodes( n0, n1, n3 ) );
  edges.push_back( edgeWithNodes( n1, n2, n4 ) );
  edges.push_back( edgeWithNodes( n2, n0, n5 ) );
  return xdm::makeRefPtr(
    new ElementTopology( empty, edges, nodes, ElementShape::Triangle, "Triangle" ) );
}

// Quadratic triangle element with 3 vertices, 3 midpoints, and 1 center point.
xdm::RefPtr< const ElementTopology > triWithNodes( std::size_t n0, std::size_t n1, std::size_t n2,
  std::size_t n3, std::size_t n4, std::size_t n5, std::size_t n6 ) {

  std::vector< xdm::RefPtr< const ElementTopology > > empty;
  std::vector< std::size_t > nodes( 7 );
  fill( nodes, n0, n1, n2, n3, n4, n5, n6 );
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  edges.push_back( edgeWithNodes( n0, n1, n3 ) );
  edges.push_back( edgeWithNodes( n1, n2, n4 ) );
  edges.push_back( edgeWithNodes( n2, n0, n5 ) );
  return xdm::makeRefPtr(
    new ElementTopology( empty, edges, nodes, ElementShape::Triangle, "Triangle" ) );
}

// Linear quadrilateral element with just 4 vertices.
xdm::RefPtr< const ElementTopology > quadWithNodes( std::size_t n0, std::size_t n1, std::size_t n2,
  std::size_t n3 ) {

  std::vector< xdm::RefPtr< const ElementTopology > > empty;
  std::vector< std::size_t > nodes( 4 );
  fill( nodes, n0, n1, n2, n3 );
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  edges.push_back( edgeWithNodes( n0, n1 ) );
  edges.push_back( edgeWithNodes( n1, n2 ) );
  edges.push_back( edgeWithNodes( n2, n3 ) );
  edges.push_back( edgeWithNodes( n3, n0 ) );
  return xdm::makeRefPtr(
    new ElementTopology( empty, edges, nodes, ElementShape::Quadrilateral, "Quadrilateral" ) );
}

// Linear quadrilateral element vertices and edge midpoints.
xdm::RefPtr< const ElementTopology > quadWithNodes( std::size_t n0, std::size_t n1, std::size_t n2,
  std::size_t n3, std::size_t n4, std::size_t n5, std::size_t n6, std::size_t n7 ) {

  std::vector< xdm::RefPtr< const ElementTopology > > empty;
  std::vector< std::size_t > nodes( 8 );
  fill( nodes, n0, n1, n2, n3, n4, n5, n6, n7 );
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  edges.push_back( edgeWithNodes( n0, n1, n4 ) );
  edges.push_back( edgeWithNodes( n1, n2, n5 ) );
  edges.push_back( edgeWithNodes( n2, n3, n6 ) );
  edges.push_back( edgeWithNodes( n3, n0, n7 ) );
  return xdm::makeRefPtr(
    new ElementTopology( empty, edges, nodes, ElementShape::Quadrilateral, "Quadrilateral" ) );
}

// Linear quadrilateral element vertices, edge midpoints, and a center point.
xdm::RefPtr< const ElementTopology > quadWithNodes( std::size_t n0, std::size_t n1, std::size_t n2,
  std::size_t n3, std::size_t n4, std::size_t n5, std::size_t n6, std::size_t n7, std::size_t n8 ) {

  std::vector< xdm::RefPtr< const ElementTopology > > empty;
  std::vector< std::size_t > nodes( 9 );
  fill( nodes, n0, n1, n2, n3, n4, n5, n6, n7, n8 );
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  edges.push_back( edgeWithNodes( n0, n1, n4 ) );
  edges.push_back( edgeWithNodes( n1, n2, n5 ) );
  edges.push_back( edgeWithNodes( n2, n3, n6 ) );
  edges.push_back( edgeWithNodes( n3, n0, n7 ) );
  return xdm::makeRefPtr(
    new ElementTopology( empty, edges, nodes, ElementShape::Quadrilateral, "Quadrilateral" ) );
}

} // namespace anon

xdm::RefPtr< const ElementTopology > elementFactory(
  const ElementShape::Type& shape,
  const std::size_t& order ) {

  // This is a double map, with lookup by shape and order. Using maps allows us to easily
  // create a void pointer for any type of element that has not yet been constructed.
  static
    std::map< ElementShape::Type,       // shape
    std::map< std::size_t,              // order
      xdm::RefPtr< const ElementTopology > > > elementMap;

  xdm::RefPtr< const ElementTopology > element = elementMap[ shape ][ order ];

  if ( ! element ) {
    switch ( shape ) {
    case ElementShape::Vertex:
      element = vertexFactory( order );
      break;
    case ElementShape::Curve:
      element = curveFactory( order );
      break;
    case ElementShape::Triangle:
      element = triangleFactory( order );
      break;
    case ElementShape::Quadrilateral:
      element = quadrilateralFactory( order );
      break;
    case ElementShape::Tetrahedron:
      element = tetrahedronFactory( order );
      break;
    case ElementShape::Pyramid:
      element = pyramidFactory( order );
      break;
    case ElementShape::Wedge:
      element = wedgeFactory( order );
      break;
    case ElementShape::Hexahedron:
      element = hexahedronFactory( order );
      break;
    }
  }

  return element;
}

xdm::RefPtr< const ElementTopology > vertexFactory(
  const std::size_t& /* order */,
  std::string name ) {

  std::vector< xdm::RefPtr< const ElementTopology > > faces;
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  std::vector< std::size_t > nodes( 1, 0 );

  if ( name.size() == 0 ) {
    name = "Polyvertex";
  }

  return xdm::makeRefPtr( new ElementTopology(
    faces,
    edges,
    nodes,
    ElementShape::Vertex,
    name ) );
}

xdm::RefPtr< const ElementTopology > curveFactory(
  const std::size_t& order,
  std::string name ) {

  if ( order < 1 || order > 2 ) {
    throw std::domain_error( "The order is not 1 or 2." );
  }

  std::vector< xdm::RefPtr< const ElementTopology > > faces;
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  std::vector< std::size_t > nodes( order + 1 );
  iota( nodes.begin(), nodes.end(), 0 );

  if ( name.size() == 0 ) {
    name = "Edge";
  }

  return xdm::makeRefPtr( new ElementTopology(
    faces,
    edges,
    nodes,
    ElementShape::Curve,
    name ) );
}

xdm::RefPtr< const ElementTopology > triangleFactory(
  const std::size_t& order,
  std::string name ) {

  if ( order < 1 || order > 2 ) {
    throw std::domain_error( "The order is not 1 or 2." );
  }

  if ( name.size() == 0 ) {
    name = "Triangle";
  }

  std::vector< xdm::RefPtr< const ElementTopology > > faces;
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  std::vector< std::size_t > nodes;

  switch ( order ) {
  case 1:
    nodes.resize( 3 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1 ) );
    edges.push_back( edgeWithNodes( 1, 2 ) );
    edges.push_back( edgeWithNodes( 2, 0 ) );
    break;
  case 2:
    nodes.resize( 7 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1, 3 ) );
    edges.push_back( edgeWithNodes( 1, 2, 4 ) );
    edges.push_back( edgeWithNodes( 2, 0, 5 ) );
    break;
  }

  return xdm::makeRefPtr( new ElementTopology(
    faces,
    edges,
    nodes,
    ElementShape::Triangle,
    name ) );
}

xdm::RefPtr< const ElementTopology > quadrilateralFactory(
  const std::size_t& order,
  std::string name ) {

  if ( order < 1 || order > 2 ) {
    throw std::domain_error( "The order is not 1 or 2." );
  }

  if ( name.size() == 0 ) {
    name = "Quadrilateral";
  }

  std::vector< xdm::RefPtr< const ElementTopology > > faces;
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  std::vector< std::size_t > nodes;

  switch ( order ) {
  case 1:
    nodes.resize( 4 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1 ) );
    edges.push_back( edgeWithNodes( 1, 2 ) );
    edges.push_back( edgeWithNodes( 2, 3 ) );
    edges.push_back( edgeWithNodes( 3, 0 ) );
    break;
  case 2:
    nodes.resize( 9 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1, 4 ) );
    edges.push_back( edgeWithNodes( 1, 2, 5 ) );
    edges.push_back( edgeWithNodes( 2, 3, 6 ) );
    edges.push_back( edgeWithNodes( 3, 0, 7 ) );
    break;
  }

  return xdm::makeRefPtr( new ElementTopology(
    faces,
    edges,
    nodes,
    ElementShape::Quadrilateral,
    name ) );
}

xdm::RefPtr< const ElementTopology > tetrahedronFactory(
  const std::size_t& order,
  std::string name ) {

  if ( order < 1 || order > 2 ) {
    throw std::domain_error( "The order is not 1 or 2." );
  }

  if ( name.size() == 0 ) {
    name = "Tetrahedron";
  }

  std::vector< xdm::RefPtr< const ElementTopology > > faces;
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  std::vector< std::size_t > nodes;

  switch ( order ) {
  case 1:
    nodes.resize( 4 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1 ) );
    edges.push_back( edgeWithNodes( 1, 2 ) );
    edges.push_back( edgeWithNodes( 2, 0 ) );
    edges.push_back( edgeWithNodes( 0, 3 ) );
    edges.push_back( edgeWithNodes( 1, 3 ) );
    edges.push_back( edgeWithNodes( 2, 3 ) );
    faces.push_back( triWithNodes( 0, 1, 3 ) );
    faces.push_back( triWithNodes( 1, 2, 3 ) );
    faces.push_back( triWithNodes( 2, 0, 3 ) );
    faces.push_back( triWithNodes( 0, 2, 1 ) );
    break;
  case 2:
    nodes.resize( 11 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1, 4 ) );
    edges.push_back( edgeWithNodes( 1, 2, 5 ) );
    edges.push_back( edgeWithNodes( 2, 0, 6 ) );
    edges.push_back( edgeWithNodes( 0, 3, 7 ) );
    edges.push_back( edgeWithNodes( 1, 3, 8 ) );
    edges.push_back( edgeWithNodes( 2, 3, 9 ) );
    faces.push_back( triWithNodes( 0, 1, 3, 4, 8, 7 ) );
    faces.push_back( triWithNodes( 1, 2, 3, 5, 9, 8 ) );
    faces.push_back( triWithNodes( 2, 0, 3, 6, 7, 9 ) );
    faces.push_back( triWithNodes( 0, 2, 1, 6, 5, 4 ) );
    break;
  }

  return xdm::makeRefPtr( new ElementTopology(
    faces,
    edges,
    nodes,
    ElementShape::Tetrahedron,
    name ) );
}

xdm::RefPtr< const ElementTopology > pyramidFactory(
  const std::size_t& order,
  std::string name ) {

  if ( order < 1 || order > 2 ) {
    throw std::domain_error( "The order is not 1 or 2." );
  }

  if ( name.size() == 0 ) {
    name = "Pyramid";
  }

  std::vector< xdm::RefPtr< const ElementTopology > > faces;
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  std::vector< std::size_t > nodes;

  switch ( order ) {
  case 1:
    nodes.resize( 5 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1 ) );
    edges.push_back( edgeWithNodes( 1, 2 ) );
    edges.push_back( edgeWithNodes( 2, 3 ) );
    edges.push_back( edgeWithNodes( 3, 0 ) );
    edges.push_back( edgeWithNodes( 0, 4 ) );
    edges.push_back( edgeWithNodes( 1, 4 ) );
    edges.push_back( edgeWithNodes( 2, 4 ) );
    edges.push_back( edgeWithNodes( 3, 4 ) );
    faces.push_back( triWithNodes( 0, 1, 4 ) );
    faces.push_back( triWithNodes( 1, 2, 4 ) );
    faces.push_back( triWithNodes( 2, 3, 4 ) );
    faces.push_back( triWithNodes( 3, 0, 4 ) );
    faces.push_back( quadWithNodes( 0, 3, 2, 1 ) );
    break;
  case 2:
    nodes.resize( 13 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1, 5 ) );
    edges.push_back( edgeWithNodes( 1, 2, 6 ) );
    edges.push_back( edgeWithNodes( 2, 3, 7 ) );
    edges.push_back( edgeWithNodes( 3, 0, 8 ) );
    edges.push_back( edgeWithNodes( 0, 4, 9 ) );
    edges.push_back( edgeWithNodes( 1, 4, 10 ) );
    edges.push_back( edgeWithNodes( 2, 4, 11 ) );
    edges.push_back( edgeWithNodes( 3, 4, 12 ) );
    faces.push_back( triWithNodes( 0, 1, 4, 5, 10, 9 ) );
    faces.push_back( triWithNodes( 1, 2, 4, 6, 11, 10 ) );
    faces.push_back( triWithNodes( 2, 3, 4, 7, 12, 11 ) );
    faces.push_back( triWithNodes( 3, 0, 4, 8, 9, 12 ) );
    faces.push_back( quadWithNodes( 0, 3, 2, 1, 8, 7, 6, 5 ) );
    break;
  }

  return xdm::makeRefPtr( new ElementTopology(
    faces,
    edges,
    nodes,
    ElementShape::Pyramid,
    name ) );
}

xdm::RefPtr< const ElementTopology > wedgeFactory(
  const std::size_t& order,
  std::string name ) {

  if ( order < 1 || order > 2 ) {
    throw std::domain_error( "The order is not 1 or 2." );
  }

  if ( name.size() == 0 ) {
    name = "Wedge";
  }

  std::vector< xdm::RefPtr< const ElementTopology > > faces;
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  std::vector< std::size_t > nodes;

  switch ( order ) {
  case 1:
    nodes.resize( 6 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1 ) );
    edges.push_back( edgeWithNodes( 1, 2 ) );
    edges.push_back( edgeWithNodes( 2, 0 ) );
    edges.push_back( edgeWithNodes( 0, 3 ) );
    edges.push_back( edgeWithNodes( 1, 4 ) );
    edges.push_back( edgeWithNodes( 2, 5 ) );
    edges.push_back( edgeWithNodes( 3, 4 ) );
    edges.push_back( edgeWithNodes( 4, 5 ) );
    edges.push_back( edgeWithNodes( 5, 3 ) );
    faces.push_back( quadWithNodes( 0, 1, 4, 3 ) );
    faces.push_back( quadWithNodes( 1, 2, 5, 4 ) );
    faces.push_back( quadWithNodes( 2, 0, 3, 5 ) );
    faces.push_back( triWithNodes( 0, 2, 1 ) );
    faces.push_back( triWithNodes( 3, 4, 5 ) );
    break;
  case 2:
    nodes.resize( 16 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1, 6 ) );
    edges.push_back( edgeWithNodes( 1, 2, 7 ) );
    edges.push_back( edgeWithNodes( 2, 0, 8 ) );
    edges.push_back( edgeWithNodes( 0, 3, 9 ) );
    edges.push_back( edgeWithNodes( 1, 4, 10 ) );
    edges.push_back( edgeWithNodes( 2, 5, 11 ) );
    edges.push_back( edgeWithNodes( 3, 4, 12 ) );
    edges.push_back( edgeWithNodes( 4, 5, 13 ) );
    edges.push_back( edgeWithNodes( 5, 3, 14 ) );
    faces.push_back( quadWithNodes( 0, 1, 4, 3, 6, 10, 12, 9 ) );
    faces.push_back( quadWithNodes( 1, 2, 5, 4, 7, 11, 13, 10 ) );
    faces.push_back( quadWithNodes( 2, 0, 3, 5, 8, 9, 14, 11 ) );
    faces.push_back( triWithNodes( 0, 2, 1, 8, 7, 6 ) );
    faces.push_back( triWithNodes( 3, 4, 5, 12, 13, 14 ) );
    break;
  }

  return xdm::makeRefPtr( new ElementTopology(
    faces,
    edges,
    nodes,
    ElementShape::Wedge,
    name ) );
}

xdm::RefPtr< const ElementTopology > hexahedronFactory(
  const std::size_t& order,
  std::string name ) {

  if ( order < 1 || order > 2 ) {
    throw std::domain_error( "The order is not 1 or 2." );
  }

  if ( name.size() == 0 ) {
    name = "Hexahedron";
  }

  std::vector< xdm::RefPtr< const ElementTopology > > faces;
  std::vector< xdm::RefPtr< const ElementTopology > > edges;
  std::vector< std::size_t > nodes;

  switch ( order ) {
  case 1:
    nodes.resize( 8 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1 ) );
    edges.push_back( edgeWithNodes( 1, 2 ) );
    edges.push_back( edgeWithNodes( 2, 3 ) );
    edges.push_back( edgeWithNodes( 3, 0 ) );
    edges.push_back( edgeWithNodes( 0, 4 ) );
    edges.push_back( edgeWithNodes( 1, 5 ) );
    edges.push_back( edgeWithNodes( 2, 6 ) );
    edges.push_back( edgeWithNodes( 3, 7 ) );
    edges.push_back( edgeWithNodes( 4, 5 ) );
    edges.push_back( edgeWithNodes( 5, 6 ) );
    edges.push_back( edgeWithNodes( 6, 7 ) );
    edges.push_back( edgeWithNodes( 7, 4 ) );
    faces.push_back( quadWithNodes( 0, 1, 5, 4 ) );
    faces.push_back( quadWithNodes( 1, 2, 6, 5 ) );
    faces.push_back( quadWithNodes( 2, 3, 7, 6 ) );
    faces.push_back( quadWithNodes( 3, 0, 4, 7 ) );
    faces.push_back( quadWithNodes( 0, 3, 2, 1 ) );
    faces.push_back( quadWithNodes( 4, 5, 6, 7 ) );
    break;
  case 2:
    nodes.resize( 21 );
    iota( nodes.begin(), nodes.end(), 0 );
    edges.push_back( edgeWithNodes( 0, 1, 8 ) );
    edges.push_back( edgeWithNodes( 1, 2, 9 ) );
    edges.push_back( edgeWithNodes( 2, 3, 10 ) );
    edges.push_back( edgeWithNodes( 3, 0, 11 ) );
    edges.push_back( edgeWithNodes( 0, 4, 12 ) );
    edges.push_back( edgeWithNodes( 1, 5, 13 ) );
    edges.push_back( edgeWithNodes( 2, 6, 14 ) );
    edges.push_back( edgeWithNodes( 3, 7, 15 ) );
    edges.push_back( edgeWithNodes( 4, 5, 16 ) );
    edges.push_back( edgeWithNodes( 5, 6, 17 ) );
    edges.push_back( edgeWithNodes( 6, 7, 18 ) );
    edges.push_back( edgeWithNodes( 7, 4, 19 ) );
    faces.push_back( quadWithNodes( 0, 1, 5, 4, 8, 13, 12, 16 ) );
    faces.push_back( quadWithNodes( 1, 2, 6, 5, 9, 14, 17, 13 ) );
    faces.push_back( quadWithNodes( 2, 3, 7, 6, 10, 15, 18, 14 ) );
    faces.push_back( quadWithNodes( 3, 0, 4, 7, 11, 12, 19, 15 ) );
    faces.push_back( quadWithNodes( 0, 3, 2, 1, 11, 10, 9, 8 ) );
    faces.push_back( quadWithNodes( 4, 5, 6, 7, 16, 17, 18, 19 ) );
    break;
  }

  return xdm::makeRefPtr( new ElementTopology(
    faces,
    edges,
    nodes,
    ElementShape::Hexahedron,
    name ) );
}

//ElementType::Type exodusElementType( std::size_t nodesPerElement, const std::string& elementName ) {
//  // Make a map that takes the number of nodes and the Exodus name and gives an elementType.
//  typedef std::map< std::string, xdmGrid::ElementType::Type > NameMap;
//  typedef std::map< std::size_t, NameMap > NumberOfNodesMap;
//  static NumberOfNodesMap nodeMap;
//  if ( nodeMap.empty() ) {
//    nodeMap[ 1 ][ "CIRCLE" ] = xdmGrid::ElementType::Circle;
//    nodeMap[ 1 ][ "SPHERE" ] = xdmGrid::ElementType::Sphere;
//    nodeMap[ 2 ][ "TRUSS" ] = xdmGrid::ElementType::Beam;
//    nodeMap[ 2 ][ "BEAM" ] = xdmGrid::ElementType::Beam;
//    nodeMap[ 2 ][ "SHELL(2D)" ] = xdmGrid::ElementType::Beam;
//    nodeMap[ 3 ][ "TRIANGLE" ] = xdmGrid::ElementType::Triangle;
//    nodeMap[ 3 ][ "TRUSS" ] = xdmGrid::ElementType::QuadraticBeam;
//    nodeMap[ 3 ][ "BEAM" ] = xdmGrid::ElementType::QuadraticBeam;
//    nodeMap[ 3 ][ "SHELL(2D)" ] = xdmGrid::ElementType::QuadraticBeam;
//    nodeMap[ 4 ][ "QUAD" ] = xdmGrid::ElementType::Quad;
//    nodeMap[ 4 ][ "SHELL(3D)" ] = xdmGrid::ElementType::Quad;
//    nodeMap[ 4 ][ "TETRA" ] = xdmGrid::ElementType::Tetra;
//    nodeMap[ 5 ][ "PYRAMID" ] = xdmGrid::ElementType::Pyramid;
//    nodeMap[ 6 ][ "WEDGE" ] = xdmGrid::ElementType::Wedge;
//    nodeMap[ 7 ][ "TRIANGLE" ] = xdmGrid::ElementType::QuadraticTriangle;
//    nodeMap[ 8 ][ "HEX" ] = xdmGrid::ElementType::Hex;
//    nodeMap[ 9 ][ "QUAD" ] = xdmGrid::ElementType::QuadraticQuad;
//    nodeMap[ 9 ][ "SHELL(3D)" ] = xdmGrid::ElementType::QuadraticQuad;
//    nodeMap[ 11 ][ "TETRA" ] = xdmGrid::ElementType::QuadraticTetra;
//    nodeMap[ 13 ][ "PYRAMID" ] = xdmGrid::ElementType::QuadraticPyramid;
//    nodeMap[ 16 ][ "WEDGE" ] = xdmGrid::ElementType::QuadraticWedge;
//    nodeMap[ 21 ][ "HEX" ] = xdmGrid::ElementType::QuadraticHex;
//  }
//
//  // Find the correct ElementType by first matching number of nodes and then matching the
//  // Exodus name.
//  NumberOfNodesMap::const_iterator matchingNodes = nodeMap.find( nodesPerElement );
//  if ( matchingNodes != nodeMap.end() ) {
//    const NameMap& matchingNames = matchingNodes->second;
//    NameMap::const_iterator matchingName = matchingNames.find( elementName );
//    if ( matchingName != matchingNames.end() ) {
//      return matchingName->second;
//    }
//  }
//
//  std::stringstream ss;
//  ss << "A element type was not found for Exodus name " << elementName << " with number of"
//    " nodes per element.";
//  throw std::runtime_error( ss.str() );
//}
//
//std::string exodusShapeString( const ElementType::Type& element ) {
//  typedef std::map< ElementShape::Type, std::string > NameMap;
//  static NameMap nameMap;
//  if ( nameMap.empty() ) {
//    nameMap[ ElementShape::Default ] = "Default";
//    nameMap[ ElementShape::Beam ] = "BEAM";
//    nameMap[ ElementShape::Circle ] = "CIRCLE";
//    nameMap[ ElementShape::Hex ] = "HEX";
//    nameMap[ ElementShape::Pyramid ] = "PYRAMID";
//    nameMap[ ElementShape::Quad ] = "QUAD";
//    nameMap[ ElementShape::Sphere ] = "SPHERE";
//    nameMap[ ElementShape::Tetra ] = "TETRA";
//    nameMap[ ElementShape::Triangle ] = "TRIANGLE";
//    nameMap[ ElementShape::Wedge ] = "WEDGE";
//  }
//
//  NameMap::const_iterator matchingName = nameMap.find( element.shape() );
//  if ( matchingName != nameMap.end() ) {
//    return matchingName->second;
//  }
//
//  std::stringstream ss;
//  ss << "An Exodus name was not found for element with name " << element.shapeName() << ".";
//  throw std::runtime_error( ss.str() );
//}

XDM_GRID_NAMESPACE_END
