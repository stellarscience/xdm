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
#define BOOST_TEST_MODULE Topology
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Topology.hpp>
#include <xdmGrid/ElementTopology.hpp>

namespace {

class NullVectorRefImpFactory : public xdm::VectorRefImpFactory< std::size_t > {
public:
  xdm::RefPtr< xdm::VectorRefImp< std::size_t > > createVectorRefImp() {
    return xdm::RefPtr< xdm::VectorRefImp< std::size_t > >();
  }
};

class ConcreteTopology : 
  public xdmGrid::Topology {
public:
  ConcreteTopology() : 
    Topology( xdm::makeRefPtr( new NullVectorRefImpFactory ) ),
    mElementTopology(
    xdmGrid::elementFactory( xdmGrid::ElementShape::Hexahedron, 1 ) ) {
  }

  virtual xdmGrid::NodeOrderingConvention::Type nodeOrdering() const {
    return xdmGrid::NodeOrderingConvention::ExodusII;
  }

  virtual xdm::RefPtr< const xdmGrid::ElementTopology > elementTopology(
    const std::size_t& elementIndex ) const {
    return mElementTopology;
  }

private:
  xdm::RefPtr< const xdmGrid::ElementTopology > mElementTopology;
};

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  ConcreteTopology t;
  xdm::XmlMetadataWrapper xml( xdm::makeRefPtr( new xdm::XmlObject ) );

  t.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Topology", xml.tag() );
}

} // namespace

