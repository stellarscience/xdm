//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmGrid/UniformGrid.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/StructuredTopology.hpp>
#include <xdmGrid/Topology.hpp>

#include <xdm/ItemVisitor.hpp>
#include <xdm/UniformDataItem.hpp>
#include <xdm/ThrowMacro.hpp>

#include <algorithm>
#include <stdexcept>

XDM_GRID_NAMESPACE_BEGIN

UniformGrid::UniformGrid() :
  Grid() {
}

UniformGrid::~UniformGrid() {
}

void UniformGrid::setGeometry( Geometry* geo ) {
  mGeometry = geo;
}

const Geometry* UniformGrid::geometry() const {
  return mGeometry;
}

Geometry* UniformGrid::geometry() {
  return mGeometry;
}

void UniformGrid::setTopology( Topology* topo ) {
  mTopology = topo;
}

Topology* UniformGrid::topology() {
  return mTopology;
}

const Topology* UniformGrid::topology() const {
  return mTopology;
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
  const UniformGrid* grid, 
  Attribute::Center center,
  Attribute::Type type,
  const std::string& name,
  xdm::primitiveType::Value dataType )
{
  xdm::RefPtr< Attribute > attribute( new Attribute( type, center ) );
  attribute->setName( name );

  // construct a UniformDataItem to hold the attribute data
  const Topology* topology = grid->topology();

  // Determine the type of topology in order to determine what it's shape
  // information looks like.
  const StructuredTopology* structuredTopology = 
    dynamic_cast< const StructuredTopology* >( topology );
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

