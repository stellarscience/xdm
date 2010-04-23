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
#define BOOST_TEST_MODULE Grid
#include <boost/test/unit_test.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Element.hpp>
#include <xdmGrid/ElementTopology.hpp>
#include <xdmGrid/Grid.hpp>

#include <xdm/PrimitiveType.hpp>
#include <xdm/RefPtr.hpp>

#include <string>

namespace {

class ConcreteGrid : public xdmGrid::Grid {
public:
  virtual xdm::RefPtr< xdmGrid::Attribute > createAttribute(
    xdmGrid::Attribute::Center center,
    xdmGrid::Attribute::Type type,
    const std::string& name,
    xdm::primitiveType::Value dataType ) {

    return xdm::RefPtr< xdmGrid::Attribute >();
  }

  virtual std::size_t numberOfElements() const { return 0; }

  virtual xdmGrid::Element element( const std::size_t& elementIndex ) const {
    return xdmGrid::Element(
      xdm::RefPtr< const xdmGrid::ElementSharedConnectivityLookup >(),
      xdm::RefPtr< const xdmGrid::ElementTopology >(),
      elementIndex );
  }
};

BOOST_AUTO_TEST_CASE( writeMetadata ) {
  ConcreteGrid g;
  g.setName( "Fred" );
  xdm::RefPtr< xdm::XmlObject > obj( new xdm::XmlObject );
  xdm::XmlMetadataWrapper xml( obj );

  g.writeMetadata( xml );

  BOOST_CHECK_EQUAL( "Grid", xml.tag() );
  BOOST_CHECK_EQUAL( "Fred", xml.attribute( "Name" ) );
}

} // namespace

