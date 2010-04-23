//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009-2010 Stellar Science. Government-purpose rights granted.
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
#include <xdmGrid/UniformGrid.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Element.hpp>
#include <xdmGrid/ElementTopology.hpp>
#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/StructuredTopology.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>

#include <xdm/ItemVisitor.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/ThrowMacro.hpp>
#include <xdm/UniformDataItem.hpp>

#include <algorithm>
#include <stdexcept>

namespace {

// A helper class for accessing elements from the geometry and topology.
class SimpleElementImp : public xdmGrid::ElementSharedConnectivityLookup {
public:
  SimpleElementImp( xdm::RefPtr< xdmGrid::Geometry > g, xdm::RefPtr< xdmGrid::Topology > t ) :
    mGeometry( g ), mTopology( t ) {}

  virtual std::size_t at( const std::size_t& elementIndex, const std::size_t& nodeIndex ) const {
    return mTopology->elementConnections( elementIndex )[ nodeIndex ];
  }

  virtual const xdmGrid::Geometry& geometry( const std::size_t& elementIndex ) const {
    return *mGeometry;
  }

private:
  xdm::RefPtr< xdmGrid::Geometry > mGeometry;
  xdm::RefPtr< xdmGrid::Topology > mTopology;
};

} // anon namespace

XDM_GRID_NAMESPACE_BEGIN

UniformGrid::UniformGrid() :
  Grid(),
  mGeometry(),
  mTopology(),
  mElementImp() {
}

UniformGrid::~UniformGrid() {
}

void UniformGrid::setGeometry( xdm::RefPtr< Geometry > geo ) {
  mGeometry = geo;
}

xdm::RefPtr< const Geometry > UniformGrid::geometry() const {
  return mGeometry;
}

xdm::RefPtr< Geometry > UniformGrid::geometry() {
  return mGeometry;
}

void UniformGrid::setTopology( xdm::RefPtr< Topology > topo ) {
  mTopology = topo;
}

xdm::RefPtr< Topology > UniformGrid::topology() {
  return mTopology;
}

xdm::RefPtr< const Topology > UniformGrid::topology() const {
  return mTopology;
}

xdm::RefPtr< xdmGrid::Attribute > UniformGrid::createAttribute(
  Attribute::Center center,
  Attribute::Type type,
  const std::string& name,
  xdm::primitiveType::Value dataType ) {

  xdm::RefPtr< Attribute > attribute( new Attribute( type, center ) );
  attribute->setName( name );

  xdm::DataShape<> attributeSpace;

  // The shape of the attribute array depends on whether we are doing dimension-by-dimension
  // access, as in structured meshes, or node-by-node and element-by-element access, as in
  // unstructured meshes.
  switch ( center ) {
    case Attribute::kNode: {
      xdm::RefPtr< const TensorProductGeometry > tpGeometry =
        xdm::dynamic_pointer_cast< const TensorProductGeometry >( mGeometry );
      if ( tpGeometry ) {
        for ( std::size_t dim = 0; dim < mGeometry->dimension(); ++dim ) {
          attributeSpace.push_back( tpGeometry->numberOfCoordinates( dim ) );
        }
      } else {
        attributeSpace.push_back( mGeometry->numberOfNodes() );
      }
      break;
    }
    case Attribute::kElement: {
      xdm::RefPtr< const StructuredTopology > structTopology =
        xdm::dynamic_pointer_cast< const StructuredTopology >( mTopology );
      if ( structTopology ) {
        attributeSpace = structTopology->shape();
      } else {
        attributeSpace.push_back( mTopology->numberOfElements() );
      }
      break;
    }
    default:
      XDM_THROW( std::logic_error(
        "Only node- and element-centered attributes are currently supported." ) );
      break;
  }

  // If the type is not a scalar, then add another dimension to the shape.
  switch( type ) {
    case Attribute::kScalar:
      //no-op
      break;
    case Attribute::kVector:
      attributeSpace.push_back( 3 );
      break;
    default:
      XDM_THROW( std::logic_error(
        "Only scalar and vector attributes are currently supported." ) );
      break;
  }

  // create the data item for the attribute values
  xdm::RefPtr< xdm::UniformDataItem > attributeDataItem(
    new xdm::UniformDataItem( dataType, attributeSpace ) );
  attribute->setDataItem( attributeDataItem );

  addAttribute( attribute );

  return attribute;
}

std::size_t UniformGrid::numberOfElements() const {
  return mTopology->numberOfElements();
}

Element UniformGrid::element( const std::size_t& elementIndex ) const
{
  if ( ! mElementImp ) {
    UniformGrid& mutableMe = const_cast< UniformGrid& >( *this );
    mutableMe.mElementImp =
      xdm::makeRefPtr( new SimpleElementImp( mutableMe.mGeometry, mutableMe.mTopology ) );
  }
  return Element( mElementImp, mTopology->elementTopology( elementIndex ), elementIndex );
}

Node UniformGrid::node( std::size_t nodeIndex ) {
  return mGeometry->node( nodeIndex );
}

ConstNode UniformGrid::node( std::size_t nodeIndex ) const {
  return mGeometry->node( nodeIndex );
}

void UniformGrid::traverse( xdm::ItemVisitor& iv ) {
  Grid::traverse( iv );

  // apply the visitor to my internal geometry and topology items
  mTopology->accept( iv );
  mGeometry->accept( iv );
}

void UniformGrid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Grid::writeMetadata( xml );

  // write Uniform grid type
  xml.setAttribute( "GridType", "Uniform" );
}

XDM_GRID_NAMESPACE_END

