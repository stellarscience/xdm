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
#ifndef xdmGrid_UnstructuredTopologyConventions_hpp
#define xdmGrid_UnstructuredTopologyConventions_hpp

#include <xdmGrid/NamespaceMacro.hpp>

#include <string>

XDM_GRID_NAMESPACE_BEGIN

namespace NodeOrderingConvention {
  enum Type {
    ExodusII = 0
  };
}

namespace ElementShape {
  enum Type {
    Default = 0,
    Beam,
    Circle,
    Hex,
    Polyvertex,
    Pyramid,
    Quad,
    Sphere,
    Tetra,
    Triangle,
    Wedge
  };
}

class ElementClass {
  ElementShape::Type mShape;
  std::size_t mNodesPerElement;
  std::string mShapeName;
public:
  ElementClass() : mShape( ElementShape::Default ), mNodesPerElement(), mShapeName( "Default" ) {}

    ElementClass( ElementShape::Type sh, std::size_t nodes, const std::string& shapeName ) :
    mShape( sh ), mNodesPerElement( nodes ), mShapeName( shapeName ) {}

  inline ElementShape::Type shape() const { return mShape; }
  inline std::size_t nodesPerElement() const { return mNodesPerElement; }
  inline std::string shapeName() const { return mShapeName; }
};

namespace ElementType {
  typedef ElementClass Type;
  const ElementClass Default;

  /// Degenerate types
  const ElementClass Circle( ElementShape::Circle, 1, "Circle" );
  const ElementClass Polyvertex( ElementShape::Polyvertex, 1, "Polyvertex" );
  const ElementClass Sphere( ElementShape::Sphere, 1, "Sphere" );

  /// Linear Elements
  const ElementClass Beam( ElementShape::Beam, 2, "Beam" );
  const ElementClass Hex( ElementShape::Hex, 8, "Hexahedron" );
  const ElementClass Pyramid( ElementShape::Pyramid, 5, "Pyramid" );
  const ElementClass Quad( ElementShape::Quad, 4, "Quadrilateral" );
  const ElementClass Tetra( ElementShape::Tetra, 4, "Tetrahedron" );
  const ElementClass Triangle( ElementShape::Triangle, 3, "Triangle" );
  const ElementClass Wedge( ElementShape::Wedge, 6, "Wedge" );

  /// Quadratic Elements
  const ElementClass QuadraticBeam( ElementShape::Beam, 3, "QuadraticBeam" );
  const ElementClass QuadraticHex( ElementShape::Hex, 21, "QuadraticHexahedron" );
  const ElementClass QuadraticPyramid( ElementShape::Pyramid, 13, "QuadraticPyramid" );
  const ElementClass QuadraticQuad( ElementShape::Quad, 9, "QuadraticQuadrilateral" );
  const ElementClass QuadraticTetra( ElementShape::Tetra, 11, "QuadraticTetrahedron" );
  const ElementClass QuadraticTriangle( ElementShape::Triangle, 7, "QuadraticTriangle" );
  const ElementClass QuadraticWedge( ElementShape::Wedge, 16, "QuadraticWedge" );
}

/// Get the ElementClass that corresponds to an Exodus shape string.
ElementType::Type exodusElementType( std::size_t nodesPerElement, const std::string& elementName );

/// Get the Exodus shape string the corresponds to an elementClass.
std::string exodusShapeString( const ElementType::Type& element );

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_UnstructuredTopologyConventions_hpp

