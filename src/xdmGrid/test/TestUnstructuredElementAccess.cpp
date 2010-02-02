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
#define BOOST_TEST_MODULE Domain
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/UnstructuredTopology.hpp>

namespace {

BOOST_AUTO_TEST_CASE( elementAccess ) {
  xdmGrid::Domain d;
  xdm::XmlMetadataWrapper xml( xdm::makeRefPtr( new xdm::XmlObject ) );

  d.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Domain", xml.tag() );

  // Assume we have a grid with unstructured mesh of tet elements. This test will iterate
  // over these elements and access node locations.
  xdmGrid::UniformGrid grid;
  xdm::RefPtr< const xdmGrid::Geometry > geom = grid.geometry();
  xdm::RefPtr< const xdmGrid::UnstructuredTopology > topo =
    dynamic_pointer_cast< const xdmGrid::UnstructuredTopology >( grid.topology() );
  BOOST_REQUIRE_EQUAL( 3, geom->dimension() );
  BOOST_CHECK_EQUAL( 4, topo->nodesPerElement() );
  BOOST_CHECK_EQUAL( xdmGrid::NodeOrderingConvention::ExodusII, topo->nodeOrdering() );
  BOOST_CHECK_EQUAL( xdmGrid::ElementShape::Tetra, topo->elementShape() );

  for ( std::size_t elementIndex = 0; elementIndex < topo->numberOfElements(); ++elementIndex ) {
    // Should we provide element iterators in UnstructuredTopology?
    const xdmGrid::Element& element = topo->element( elementIndex );

    // element.getQuadraturePoints()
    // evaluate attributes at quad points
    // setup local siffness, forcing arrays
    for( std::size_t quadIndex q = 0; quadIndex < element->numberOfQuadPoints(); ++quadIndex ) {
      for( std::size_t shape0Index = 0; shape0Index < topo->nodesPerElement(); ++shape0Index ) {
        for( std::size_t shape1Index = 0; shape1Index < topo->nodesPerElement(); ++shape1Index ) {
          K( shape0Index, shape1Index ) += element->getJxW( quadIndex ) *
            element->shapeGradient( quadIndex, shape0Index ) * conductivityTensor[ quadIndex ] *
            element->shapeGradient( quadIndex, shape1Index );
      }
      F( shape0Index ) += element->getJxW( quadIndex ) * source[ quadIndex ];
    }

    // Put the local K, F into the global K, F.
    element->applyLocalToGlobal( K, F, Kglobal, Fglobal );
  }

  // Solve it.
}

} // namespace

