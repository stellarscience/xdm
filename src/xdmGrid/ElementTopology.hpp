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
#ifndef xdmGrid_ElementTopology_hpp
#define xdmGrid_ElementTopology_hpp

#include <xdmGrid/NamespaceMacro.hpp>

#include <xdm/Forward.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <cassert>
#include <string>
#include <vector>

XDM_GRID_NAMESPACE_BEGIN

namespace NodeOrderingConvention {
  enum Type {
    ExodusII = 0
  };
}

namespace ElementShape {
  enum Type {
    Vertex,
    Curve,
    Triangle,
    Quadrilateral,
    Tetrahedron,
    Pyramid,
    Wedge,
    Hexahedron
  };
}

namespace ElementDimension {
  enum Type {
    Point,
    Curve,
    Surface,
    Volume
  };
}

class ElementTopology : public xdm::ReferencedObject {
public:
  inline ElementShape::Type shape() const { return mShape; }
  inline std::string name() const { return mName; }

  inline std::size_t numberOfFaces() const { return mFaces.size(); }
  inline std::size_t numberOfEdges() const { return mEdges.size(); }
  inline std::size_t numberOfNodes() const { return mNodes.size(); }

  inline xdm::RefPtr< const ElementTopology > face( std::size_t faceIndex ) const {
    assert( faceIndex < mFaces.size() );
    return mFaces[ faceIndex ];
  }

  inline xdm::RefPtr< const ElementTopology > edge( std::size_t edgeIndex ) const {
    assert( edgeIndex < mEdges.size() );
    return mEdges[ edgeIndex ];
  }

  inline std::size_t node( std::size_t nodeIndex ) const {
    assert( nodeIndex < mNodes.size() );
    return mNodes[ nodeIndex ];
  }

  ElementTopology(
    const std::vector< xdm::RefPtr< const ElementTopology > >& faces,
    const std::vector< xdm::RefPtr< const ElementTopology > >& edges,
    const std::vector< std::size_t >& nodes,
    const ElementShape::Type& shape,
    const std::string& name ) :
      mFaces( faces ), mEdges( edges ), mNodes( nodes ), mShape( shape ), mName( name ) {
  }

private:
  std::vector< xdm::RefPtr< const ElementTopology > > mFaces;
  std::vector< xdm::RefPtr< const ElementTopology > > mEdges;
  std::vector< std::size_t > mNodes;
  ElementShape::Type mShape;
  std::string mName;
};

xdm::RefPtr< const ElementTopology > elementFactory(
  const ElementShape::Type& shape,
  const std::size_t& order );

xdm::RefPtr< const ElementTopology > vertexFactory(
  const std::size_t& order,
  std::string name = "" );

xdm::RefPtr< const ElementTopology > curveFactory(
  const std::size_t& order,
  std::string name = "" );

xdm::RefPtr< const ElementTopology > triangleFactory(
  const std::size_t& order,
  std::string name = "" );

xdm::RefPtr< const ElementTopology > quadrilateralFactory(
  const std::size_t& order,
  std::string name = "" );

xdm::RefPtr< const ElementTopology > tetrahedronFactory(
  const std::size_t& order,
  std::string name = "" );

xdm::RefPtr< const ElementTopology > pyramidFactory(
  const std::size_t& order,
  std::string name = "" );

xdm::RefPtr< const ElementTopology > wedgeFactory(
  const std::size_t& order,
  std::string name = "" );

xdm::RefPtr< const ElementTopology > hexahedronFactory(
  const std::size_t& order,
  std::string name = "" );

ElementDimension::Type elementDimension( const ElementShape::Type& shape );

///// Get the ElementTopology that corresponds to an Exodus shape string.
//xdm::RefPtr< const ElementTopology > exodusElementType(
//  std::size_t nodesPerElement,
//  const std::string& elementName );
//
///// Get the Exodus shape string the corresponds to an elementClass.
//std::string exodusShapeString( const ElementType::Type& element );

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_ElementTopology_hpp

