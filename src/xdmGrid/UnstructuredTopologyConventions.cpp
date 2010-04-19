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

ElementType::Type exodusElementType( std::size_t nodesPerElement, const std::string& elementName ) {
  // Make a map that takes the number of nodes and the Exodus name and gives an elementType.
  typedef std::map< std::string, xdmGrid::ElementType::Type > NameMap;
  typedef std::map< std::size_t, NameMap > NumberOfNodesMap;
  static NumberOfNodesMap nodeMap;
  if ( nodeMap.empty() ) {
    nodeMap[ 1 ][ "CIRCLE" ] = xdmGrid::ElementType::Circle;
    nodeMap[ 1 ][ "SPHERE" ] = xdmGrid::ElementType::Sphere;
    nodeMap[ 2 ][ "TRUSS" ] = xdmGrid::ElementType::Beam;
    nodeMap[ 2 ][ "BEAM" ] = xdmGrid::ElementType::Beam;
    nodeMap[ 2 ][ "SHELL(2D)" ] = xdmGrid::ElementType::Beam;
    nodeMap[ 3 ][ "TRIANGLE" ] = xdmGrid::ElementType::Triangle;
    nodeMap[ 3 ][ "TRUSS" ] = xdmGrid::ElementType::QuadraticBeam;
    nodeMap[ 3 ][ "BEAM" ] = xdmGrid::ElementType::QuadraticBeam;
    nodeMap[ 3 ][ "SHELL(2D)" ] = xdmGrid::ElementType::QuadraticBeam;
    nodeMap[ 4 ][ "QUAD" ] = xdmGrid::ElementType::Quad;
    nodeMap[ 4 ][ "SHELL(3D)" ] = xdmGrid::ElementType::Quad;
    nodeMap[ 4 ][ "TETRA" ] = xdmGrid::ElementType::Tetra;
    nodeMap[ 5 ][ "PYRAMID" ] = xdmGrid::ElementType::Pyramid;
    nodeMap[ 6 ][ "WEDGE" ] = xdmGrid::ElementType::Wedge;
    nodeMap[ 7 ][ "TRIANGLE" ] = xdmGrid::ElementType::QuadraticTriangle;
    nodeMap[ 8 ][ "HEX" ] = xdmGrid::ElementType::Hex;
    nodeMap[ 9 ][ "QUAD" ] = xdmGrid::ElementType::QuadraticQuad;
    nodeMap[ 9 ][ "SHELL(3D)" ] = xdmGrid::ElementType::QuadraticQuad;
    nodeMap[ 11 ][ "TETRA" ] = xdmGrid::ElementType::QuadraticTetra;
    nodeMap[ 13 ][ "PYRAMID" ] = xdmGrid::ElementType::QuadraticPyramid;
    nodeMap[ 16 ][ "WEDGE" ] = xdmGrid::ElementType::QuadraticWedge;
    nodeMap[ 21 ][ "HEX" ] = xdmGrid::ElementType::QuadraticHex;
  }

  // Find the correct ElementType by first matching number of nodes and then matching the
  // Exodus name.
  NumberOfNodesMap::const_iterator matchingNodes = nodeMap.find( nodesPerElement );
  if ( matchingNodes != nodeMap.end() ) {
    const NameMap& matchingNames = matchingNodes->second;
    NameMap::const_iterator matchingName = matchingNames.find( elementName );
    if ( matchingName != matchingNames.end() ) {
      return matchingName->second;
    }
  }

  std::stringstream ss;
  ss << "A element type was not found for Exodus name " << elementName << " with number of"
    " nodes per element.";
  throw std::runtime_error( ss.str() );
}

std::string exodusShapeString( const ElementType::Type& element ) {
  typedef std::map< ElementShape::Type, std::string > NameMap;
  static NameMap nameMap;
  if ( nameMap.empty() ) {
    nameMap[ ElementShape::Default ] = "Default";
    nameMap[ ElementShape::Beam ] = "BEAM";
    nameMap[ ElementShape::Circle ] = "CIRCLE";
    nameMap[ ElementShape::Hex ] = "HEX";
    nameMap[ ElementShape::Pyramid ] = "PYRAMID";
    nameMap[ ElementShape::Quad ] = "QUAD";
    nameMap[ ElementShape::Sphere ] = "SPHERE";
    nameMap[ ElementShape::Tetra ] = "TETRA";
    nameMap[ ElementShape::Triangle ] = "TRIANGLE";
    nameMap[ ElementShape::Wedge ] = "WEDGE";
  }

  NameMap::const_iterator matchingName = nameMap.find( element.shape() );
  if ( matchingName != nameMap.end() ) {
    return matchingName->second;
  }

  std::stringstream ss;
  ss << "An Exodus name was not found for element with name " << element.shapeName() << ".";
  throw std::runtime_error( ss.str() );
}

XDM_GRID_NAMESPACE_END
