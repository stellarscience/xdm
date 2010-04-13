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
#include <xdmGrid/Cell.hpp>
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

// A helper class for accessing cells from the geometry and topology.
class SimpleCellImp : public xdmGrid::CellSharedImp {
public:
  SimpleCellImp( xdm::RefPtr< xdmGrid::Geometry > g, xdm::RefPtr< xdmGrid::Topology > t ) :
    mGeometry( g ), mTopology( t ) {}

  virtual xdmGrid::ConstNode node( std::size_t cellIndex, std::size_t nodeIndex ) const {
    return mGeometry->node( mTopology->cellConnections( cellIndex )[ nodeIndex ] );
  }

  virtual xdmGrid::CellType::Type cellType( std::size_t cellIndex ) const {
    return mTopology->cellType( cellIndex );
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
  mCellImp() {
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

Cell UniformGrid::cell( std::size_t cellIndex )
{
  if ( ! mCellImp ) {
    setCellSharedImp( xdm::makeRefPtr( new SimpleCellImp( mGeometry, mTopology ) ) );
  }
  return Cell( mCellImp, cellIndex );
}

ConstCell UniformGrid::cell( std::size_t cellIndex ) const
{
  if ( ! mCellImp ) {
    UniformGrid& mutableThis = const_cast< UniformGrid& >( *this );
    mutableThis.setCellSharedImp(
      xdm::makeRefPtr( new SimpleCellImp( mutableThis.mGeometry, mutableThis.mTopology ) ) );
  }
  return ConstCell( mCellImp, cellIndex );
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

void UniformGrid::setCellSharedImp( xdm::RefPtr< CellSharedImp > cellImp ) {
  mCellImp = cellImp;
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
  // access, as in structured meshes, or node-by-node and cell-by-cell access, as in
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
    case Attribute::kCell: {
      xdm::RefPtr< const StructuredTopology > structTopology =
        xdm::dynamic_pointer_cast< const StructuredTopology >( topology );
      if ( structTopology ) {
        attributeSpace = structTopology->shape();
      } else {
        attributeSpace.push_back( topology->numberOfCells() );
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

