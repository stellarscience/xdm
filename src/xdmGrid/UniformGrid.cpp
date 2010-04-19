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
#include <xdmGrid/UniformGrid.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Element.hpp>
#include <xdmGrid/StructuredTopology.hpp>
#include <xdmGrid/TensorProductGeometry.hpp>
#include <xdmGrid/UnstructuredTopologyConventions.hpp>

#include <xdm/ItemVisitor.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/ThrowMacro.hpp>

#include <algorithm>
#include <stdexcept>

namespace {

// A helper class for accessing elements from the geometry and topology.
class SimpleElementImp : public xdmGrid::ElementSharedImp {
public:
  SimpleElementImp( xdm::RefPtr< xdmGrid::Geometry > g, xdm::RefPtr< xdmGrid::Topology > t ) :
    mGeometry( g ), mTopology( t ) {}

  virtual xdmGrid::ConstNode node( std::size_t elementIndex, std::size_t nodeIndex ) const {
    return mGeometry->node( mTopology->elementConnections( elementIndex )[ nodeIndex ] );
  }

  virtual xdmGrid::ElementType::Type elementType( std::size_t elementIndex ) const {
    return mTopology->elementType( elementIndex );
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

void UniformGrid::addAttribute( xdm::RefPtr< Attribute > attribute ) {
  appendChild( attribute );
}

xdm::RefPtr< const Attribute > UniformGrid::attributeByIndex( std::size_t index ) const {
  if ( index < numberOfChildren() ) {
    return child( index );
  } else {
    return xdm::RefPtr< const Attribute >();
  }
}

xdm::RefPtr< Attribute > UniformGrid::attributeByIndex( std::size_t index ) {
  return xdm::const_pointer_cast< Attribute >(
    static_cast< const UniformGrid& >(*this).attributeByIndex( index )
  );
}

xdm::RefPtr< const Attribute >
UniformGrid::attributeByName( const std::string& name ) const {
  for ( int i = 0; i < numberOfChildren(); ++i ) {
    if ( child( i )->name() == name ) return child( i );
  }
  return xdm::RefPtr< const Attribute >();
}

xdm::RefPtr< Attribute >
UniformGrid::attributeByName( const std::string& name ) {
  return xdm::const_pointer_cast< Attribute >(
    static_cast< const UniformGrid& >(*this).attributeByName( name )
  );
}

Element UniformGrid::element( std::size_t elementIndex )
{
  if ( ! mElementImp ) {
    setElementSharedImp( xdm::makeRefPtr( new SimpleElementImp( mGeometry, mTopology ) ) );
  }
  return Element( mElementImp, elementIndex );
}

ConstElement UniformGrid::element( std::size_t elementIndex ) const
{
  if ( ! mElementImp ) {
    UniformGrid& mutableThis = const_cast< UniformGrid& >( *this );
    mutableThis.setElementSharedImp(
      xdm::makeRefPtr( new SimpleElementImp( mutableThis.mGeometry, mutableThis.mTopology ) ) );
  }
  return ConstElement( mElementImp, elementIndex );
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
  std::for_each(
    xdm::begin< Attribute >( *this ),
    xdm::end< Attribute >( *this ),
    xdm::ApplyVisitor( iv ) );
}

void UniformGrid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Grid::writeMetadata( xml );

  // write Uniform grid type
  xml.setAttribute( "GridType", "Uniform" );
}

void UniformGrid::setElementSharedImp( xdm::RefPtr< ElementSharedImp > elementImp ) {
  mElementImp = elementImp;
}

xdm::RefPtr< xdmGrid::Attribute >
createAttribute(
  xdm::RefPtr< const UniformGrid > grid,
  Attribute::Center center,
  Attribute::Type type,
  const std::string& name,
  xdm::primitiveType::Value dataType )
{
  xdm::RefPtr< Attribute > attribute( new Attribute( type, center ) );
  attribute->setName( name );

  xdm::RefPtr< const Geometry > geometry = grid->geometry();
  xdm::RefPtr< const Topology > topology = grid->topology();
  xdm::DataShape<> attributeSpace;

  // The shape of the attribute array depends on whether we are doing dimension-by-dimension
  // access, as in structured meshes, or node-by-node and element-by-element access, as in
  // unstructured meshes.
  switch ( center ) {
    case Attribute::kNode: {
      xdm::RefPtr< const TensorProductGeometry > tpGeometry =
        xdm::dynamic_pointer_cast< const TensorProductGeometry >( geometry );
      if ( tpGeometry ) {
        for ( std::size_t dim = 0; dim < geometry->dimension(); ++dim ) {
          attributeSpace.push_back( tpGeometry->numberOfCoordinates( dim ) );
        }
      } else {
        attributeSpace.push_back( geometry->numberOfNodes() );
      }
      break;
    }
    case Attribute::kElement: {
      xdm::RefPtr< const StructuredTopology > structTopology =
        xdm::dynamic_pointer_cast< const StructuredTopology >( topology );
      if ( structTopology ) {
        attributeSpace = structTopology->shape();
      } else {
        attributeSpace.push_back( topology->numberOfElements() );
      }
      break;
    }
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
        "Only scalar and vector attributes are currently supported" ) );
      break;
  }

  // create the data item for the attribute values
  xdm::RefPtr< xdm::UniformDataItem > attributeDataItem(
    new xdm::UniformDataItem( dataType, attributeSpace ) );
  attribute->setDataItem( attributeDataItem );

  return attribute;
}

XDM_GRID_NAMESPACE_END

