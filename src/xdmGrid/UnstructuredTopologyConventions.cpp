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
#include <xdmGrid/UnstructuredTopologyConventions.hpp>

#include <map>
#include <sstream>
#include <stdexcept>

XDM_GRID_NAMESPACE_BEGIN

CellType::Type exodusCellType( std::size_t nodesPerCell, const std::string& cellName ) {
  // Make a map that takes the number of nodes and the Exodus name and gives a CellType.
  typedef std::map< std::string, xdmGrid::CellType::Type > NameMap;
  typedef std::map< std::size_t, NameMap > NumberOfNodesMap;
  static NumberOfNodesMap nodeMap;
  if ( nodeMap.empty() ) {
    nodeMap[ 1 ][ "CIRCLE" ] = xdmGrid::CellType::Circle;
    nodeMap[ 1 ][ "SPHERE" ] = xdmGrid::CellType::Sphere;
    nodeMap[ 2 ][ "TRUSS" ] = xdmGrid::CellType::Beam;
    nodeMap[ 2 ][ "BEAM" ] = xdmGrid::CellType::Beam;
    nodeMap[ 2 ][ "SHELL(2D)" ] = xdmGrid::CellType::Beam;
    nodeMap[ 3 ][ "TRIANGLE" ] = xdmGrid::CellType::Triangle;
    nodeMap[ 3 ][ "TRUSS" ] = xdmGrid::CellType::QuadraticBeam;
    nodeMap[ 3 ][ "BEAM" ] = xdmGrid::CellType::QuadraticBeam;
    nodeMap[ 3 ][ "SHELL(2D)" ] = xdmGrid::CellType::QuadraticBeam;
    nodeMap[ 4 ][ "QUAD" ] = xdmGrid::CellType::Quad;
    nodeMap[ 4 ][ "SHELL(3D)" ] = xdmGrid::CellType::Quad;
    nodeMap[ 4 ][ "TETRA" ] = xdmGrid::CellType::Tetra;
    nodeMap[ 5 ][ "PYRAMID" ] = xdmGrid::CellType::Pyramid;
    nodeMap[ 6 ][ "WEDGE" ] = xdmGrid::CellType::Wedge;
    nodeMap[ 7 ][ "TRIANGLE" ] = xdmGrid::CellType::QuadraticTriangle;
    nodeMap[ 8 ][ "HEX" ] = xdmGrid::CellType::Hex;
    nodeMap[ 9 ][ "QUAD" ] = xdmGrid::CellType::QuadraticQuad;
    nodeMap[ 9 ][ "SHELL(3D)" ] = xdmGrid::CellType::QuadraticQuad;
    nodeMap[ 11 ][ "TETRA" ] = xdmGrid::CellType::QuadraticTetra;
    nodeMap[ 13 ][ "PYRAMID" ] = xdmGrid::CellType::QuadraticPyramid;
    nodeMap[ 16 ][ "WEDGE" ] = xdmGrid::CellType::QuadraticWedge;
    nodeMap[ 21 ][ "HEX" ] = xdmGrid::CellType::QuadraticHex;
  }

  // Find the correct CellType by first matching number of nodes and then matching the
  // Exodus name.
  NumberOfNodesMap::const_iterator matchingNodes = nodeMap.find( nodesPerCell );
  if ( matchingNodes != nodeMap.end() ) {
    const NameMap& matchingNames = matchingNodes->second;
    NameMap::const_iterator matchingName = matchingNames.find( cellName );
    if ( matchingName != matchingNames.end() ) {
      return matchingName->second;
    }
  }

  std::stringstream ss;
  ss << "A cell type was not found for Exodus name " << cellName << " with number of"
    " nodes per cell.";
  throw std::runtime_error( ss.str() );
}

std::string exodusShapeString( const CellType::Type& cell ) {
  typedef std::map< CellShape::Type, std::string > NameMap;
  static NameMap nameMap;
  if ( nameMap.empty() ) {
    nameMap[ CellShape::Default ] = "Default";
    nameMap[ CellShape::Beam ] = "BEAM";
    nameMap[ CellShape::Circle ] = "CIRCLE";
    nameMap[ CellShape::Hex ] = "HEX";
    nameMap[ CellShape::Pyramid ] = "PYRAMID";
    nameMap[ CellShape::Quad ] = "QUAD";
    nameMap[ CellShape::Sphere ] = "SPHERE";
    nameMap[ CellShape::Tetra ] = "TETRA";
    nameMap[ CellShape::Triangle ] = "TRIANGLE";
    nameMap[ CellShape::Wedge ] = "WEDGE";
  }

  NameMap::const_iterator matchingName = nameMap.find( cell.shape() );
  if ( matchingName != nameMap.end() ) {
    return matchingName->second;
  }

  std::stringstream ss;
  ss << "An Exodus name was not found for cell with name " << cell.shapeName() << ".";
  throw std::runtime_error( ss.str() );
}

XDM_GRID_NAMESPACE_END
