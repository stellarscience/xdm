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
#define BOOST_TEST_MODULE MultiArrayGeometry
#include <boost/test/unit_test.hpp>

#include <xdmGrid/test/Cube.hpp>

#include <xdmGrid/MultiArrayGeometry.hpp>

#include <xdm/test/TestHelpers.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::MultiArrayGeometry g(3);
  xdm::XmlMetadataWrapper xml( xdm::makeRefPtr( new xdm::XmlObject ) );

  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Geometry", xml.tag() );
  BOOST_CHECK_EQUAL( "X_Y_Z", xml.attribute( "GeometryType" ) );
}

BOOST_AUTO_TEST_CASE( nodeAccess ) {
  xdmGrid::MultiArrayGeometry g(3);

  CubeOfTets cube;
  xdm::RefPtr< xdm::UniformDataItem > nodeListX = test::createUniformDataItem(
    cube.nodeX(), cube.numberOfNodes(), xdm::primitiveType::kDouble );
  xdm::RefPtr< xdm::UniformDataItem > nodeListY = test::createUniformDataItem(
    cube.nodeY(), cube.numberOfNodes(), xdm::primitiveType::kDouble );
  xdm::RefPtr< xdm::UniformDataItem > nodeListZ = test::createUniformDataItem(
    cube.nodeZ(), cube.numberOfNodes(), xdm::primitiveType::kDouble );
  g.setCoordinateValues( 0, nodeListX );
  g.setCoordinateValues( 1, nodeListY );
  g.setCoordinateValues( 2, nodeListZ );

  // Check node2. xyz is 1,1,0
  BOOST_CHECK_EQUAL( 1., g.node( 2 )[0] );
  BOOST_CHECK_EQUAL( 1., g.node( 2 )[1] );
  BOOST_CHECK_EQUAL( 0., g.node( 2 )[2] );

  // Check node7. xyz is 0,1,1
  BOOST_CHECK_EQUAL( 0., g.node( 7 )[0] );
  BOOST_CHECK_EQUAL( 1., g.node( 7 )[1] );
  BOOST_CHECK_EQUAL( 1., g.node( 7 )[2] );
}

} // namespace

