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
#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/StructuredTopology.hpp>
#include <xdmGrid/Topology.hpp>
#include <xdmGrid/UnstructuredTopologyConventions.hpp>

#include <xdm/ItemVisitor.hpp>
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

Cell UniformGrid::cell( std::size_t cellIndex )
{
  if ( ! mCellImp ) {
    mCellImp = new SimpleCellImp( mGeometry, mTopology );
  }
  return Cell( mCellImp, cellIndex );
}

ConstCell UniformGrid::cell( std::size_t cellIndex ) const
{
  if ( ! mCellImp ) {
    UniformGrid& mutableThis = const_cast< UniformGrid& >( *this );
    mutableThis.mCellImp = new SimpleCellImp( mutableThis.mGeometry, mutableThis.mTopology );
  }
  return ConstCell( mCellImp, cellIndex );
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

  // construct a UniformDataItem to hold the attribute data
  xdm::RefPtr< const Topology > topology = grid->topology();

  // Determine the type of topology in order to determine what it's shape
  // information looks like.
  xdm::RefPtr< const StructuredTopology > structuredTopology =
    xdm::dynamic_pointer_cast< const StructuredTopology >( topology );
  if ( structuredTopology ) {
    xdm::DataShape<> attributeSpace;

    // generate the base space for the attribute depending on it's center
    switch( center ) {
    case Attribute::kNode:
      attributeSpace = structuredTopology->shape();
      break;
    case Attribute::kCell:
      attributeSpace = xdm::makeContraction( structuredTopology->shape(), 1 );
      break;
    default:
      XDM_THROW( std::logic_error(
        "Cell or node centered values are currently only supported" ) );
      break;
    }

    // append a dimension if necessary depending upon the attribute type
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

  } else {
    XDM_THROW( std::logic_error(
      "Unstructured topologies are not yet supported" ) );
  }
  return attribute;
}

XDM_GRID_NAMESPACE_END

