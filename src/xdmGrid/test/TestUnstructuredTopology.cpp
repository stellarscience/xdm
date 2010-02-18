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
#define BOOST_TEST_MODULE UnstructuredTopology
#include <boost/test/unit_test.hpp>

#include <xdmGrid/UnstructuredTopology.hpp>

#include <xdm/test/TestHelpers.hpp>

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  xdmGrid::UnstructuredTopology t;
  xdm::XmlMetadataWrapper xml( xdm::makeRefPtr( new xdm::XmlObject ) );

  t.setNumberOfCells( 42 );
  t.setCellType( xdmGrid::CellType::Tetra );

  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Topology", xml.tag() );
  BOOST_CHECK_EQUAL( "42", xml.attribute( "NumberOfElements" ) );
  BOOST_CHECK_EQUAL( "4", xml.attribute( "NodesPerElement" ) );
}

