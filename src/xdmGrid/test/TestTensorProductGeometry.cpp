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
#define BOOST_TEST_MODULE TensorProductGeometry
#include <boost/test/unit_test.hpp>

#include <xdmGrid/test/Cube.hpp>

#include <xdmGrid/TensorProductGeometry.hpp>

#include <xdm/test/TestHelpers.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::TensorProductGeometry g(3);
  xdm::XmlMetadataWrapper xml( xdm::makeRefPtr( new xdm::XmlObject ) );

  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Geometry", xml.tag() );
  BOOST_CHECK_EQUAL( "VxVyVz", xml.attribute( "GeometryType" ) );
}

BOOST_AUTO_TEST_CASE( nodeAccess3D ) {
  xdmGrid::TensorProductGeometry g(3);

  StructuredCube cube;
  xdm::RefPtr< xdm::UniformDataItem > xAxis = test::createUniformDataItem(
    cube.axis( 0 ), cube.axisSize( 0 ), xdm::primitiveType::kDouble );
  xdm::RefPtr< xdm::UniformDataItem > yAxis = test::createUniformDataItem(
    cube.axis( 1 ), cube.axisSize( 1 ), xdm::primitiveType::kDouble );
  xdm::RefPtr< xdm::UniformDataItem > zAxis = test::createUniformDataItem(
    cube.axis( 2 ), cube.axisSize( 2 ), xdm::primitiveType::kDouble );

  g.setCoordinateValues( 0, xAxis );
  g.setCoordinateValues( 1, yAxis );
  g.setCoordinateValues( 2, zAxis );

  BOOST_CHECK_EQUAL( cube.numberOfNodes(), g.numberOfNodes() );

  // Check node2. xyz is 0.75, 0, 0
  BOOST_CHECK_CLOSE( 0.75, g.node( 2 )[0], 1.e-8 );
  BOOST_CHECK_CLOSE( 0.00, g.node( 2 )[1], 1.e-8 );
  BOOST_CHECK_CLOSE( 0.00, g.node( 2 )[2], 1.e-8 );

  // Check node7. xyz is 1.0, 0.4, 0
  BOOST_CHECK_CLOSE( 1.0, g.node( 7 )[0], 1.e-8 );
  BOOST_CHECK_CLOSE( 0.4, g.node( 7 )[1], 1.e-8 );
  BOOST_CHECK_CLOSE( 0.0, g.node( 7 )[2], 1.e-8 );

  // Check node17. xyz is 0.25, 0.4, 0.3
  BOOST_CHECK_CLOSE( 0.25, g.node( 17 )[0], 1.e-8 );
  BOOST_CHECK_CLOSE( 0.40, g.node( 17 )[1], 1.e-8 );
  BOOST_CHECK_CLOSE( 0.30, g.node( 17 )[2], 1.e-8 );
}

} // namespace

