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

namespace CellShape {
  enum Type {
    Default = 0,
    Circle,
    Beam,
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

class CellClass {
  CellShape::Type mShape;
  std::size_t mNodesPerCell;
  std::string mShapeName;
public:
  CellClass() : mShape( CellShape::Default ), mNodesPerCell(), mShapeName( "Default" ) {}

    CellClass( CellShape::Type sh, std::size_t nodes, const std::string& shapeName ) :
    mShape( sh ), mNodesPerCell( nodes ), mShapeName( shapeName ) {}

  inline CellShape::Type shape() const { return mShape; }
  inline std::size_t nodesPerCell() const { return mNodesPerCell; }
  inline std::string shapeName() const { return mShapeName; }
};

namespace CellType {
  typedef CellClass Type;
  const CellClass Default;

  /// Degenerate types
  const CellClass Circle( CellShape::Circle, 1, "Circle" );
  const CellClass Polyvertex( CellShape::Polyvertex, 1, "Polyvertex" );
  const CellClass Sphere( CellShape::Sphere, 1, "Sphere" );

  /// Linear cells
  const CellClass Beam( CellShape::Beam, 2, "Beam" );
  const CellClass Hex( CellShape::Hex, 8, "Hexahedron" );
  const CellClass Pyramid( CellShape::Pyramid, 5, "Pyramid" );
  const CellClass Quad( CellShape::Quad, 4, "Quadrilateral" );
  const CellClass Tetra( CellShape::Tetra, 4, "Tetrahedron" );
  const CellClass Triangle( CellShape::Triangle, 3, "Triangle" );
  const CellClass Wedge( CellShape::Wedge, 6, "Wedge" );

  /// Quadratic cells
  const CellClass QuadraticBeam( CellShape::Beam, 3, "QuadraticBeam" );
  const CellClass QuadraticHex( CellShape::Hex, 21, "QuadraticHexahedron" );
  const CellClass QuadraticPyramid( CellShape::Pyramid, 13, "QuadraticPyramid" );
  const CellClass QuadraticQuad( CellShape::Quad, 9, "QuadraticQuadrilateral" );
  const CellClass QuadraticTetra( CellShape::Tetra, 11, "QuadraticTetrahedron" );
  const CellClass QuadraticTriangle( CellShape::Triangle, 7, "QuadraticTriangle" );
  const CellClass QuadraticWedge( CellShape::Wedge, 16, "QuadraticWedge" );
}

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_UnstructuredTopologyConventions_hpp

