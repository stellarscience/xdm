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
#include <xdmGrid/CollectionGrid.hpp>

#include <xdmGrid/Element.hpp>
#include <xdmGrid/ElementTopology.hpp>
#include <xdmGrid/Topology.hpp>

#include <xdm/DataShape.hpp>
#include <xdm/ItemVisitor.hpp>
#include <xdm/UniformDataItem.hpp>

#include <algorithm>
#include <stdexcept>
#include <utility>

#include <xdm/ThrowMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

CollectionGrid::CollectionGrid( CollectionType type ) :
  Grid(),
  mGrids(),
  mArrays(),
  mElementOffsets(),
  mType( type ) {
}

CollectionGrid::~CollectionGrid() {
}

void CollectionGrid::setType( CollectionType t ) {
  mType = t;
}

CollectionGrid::CollectionType CollectionGrid::type() const {
  return mType;
}

void CollectionGrid::appendGrid(
  xdm::RefPtr< Grid > grid,
  xdm::RefPtr< xdm::UniformDataItem > elementIndices ) {

  if ( elementIndices && elementIndices->dataspace().rank() != 1 ) {
    XDM_THROW( std::invalid_argument( "The rank of the element indices array is not 1." ) );
  }

  if ( grid.get() == this ) {
    XDM_THROW( std::invalid_argument(
      "A CollectionGrid attempted to add itself to its own collection." ) );
  }

  mGrids.push_back( grid );
  mArrays.push_back( elementIndices );
}

xdm::RefPtr< xdmGrid::Attribute > CollectionGrid::createAttribute(
  Attribute::Center center,
  Attribute::Type type,
  const std::string& name,
  xdm::primitiveType::Value dataType ) {

  if ( center == Attribute::kNode ) {
    XDM_THROW( std::invalid_argument(
      "Node-centered attributes are not allowed for CollectionGrids." ) );
  }

  xdm::RefPtr< Attribute > attribute( new Attribute( type, center ) );
  attribute->setName( name );

  xdm::DataShape<> attributeSpace;

  // CollectionGrid element access is inherently unstructred access.
  if ( center != Attribute::kElement ) {
    XDM_THROW( std::logic_error( "Only element-centered attributes are currently supported." ) );
  }
  attributeSpace.push_back( numberOfElements() );

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

std::size_t CollectionGrid::numberOfElements() const {
  std::size_t elementCount = 0;
  for ( std::size_t i = 0; i < mArrays.size(); ++i ) {
    if ( mArrays[i] ) {
      elementCount += mArrays[i]->dataspace()[0];
    } else {
      // If the array does not exist, then we assume that the whole grid is being used.
      elementCount += mGrids[i]->numberOfElements();
    }
  }
  return elementCount;
}

Element CollectionGrid::element( const std::size_t& elementIndex ) const
{
  std::pair< Grid*, std::size_t > found = findGrid( elementIndex );
  return found.first->element( found.second );
}

void CollectionGrid::traverse( xdm::ItemVisitor& iv ) {
  Grid::traverse( iv );

  std::for_each( mGrids.begin(), mGrids.end(), xdm::ApplyVisitor( iv ) );
  std::for_each( mArrays.begin(), mArrays.end(), xdm::ApplyVisitor( iv ) );
}

void CollectionGrid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Grid::writeMetadata( xml );

  // write the grid type as a collection
  xml.setAttribute( "GridType", "Collection" );

  // write the type of collection.
  switch ( mType ) {
  case kSpatial:
    xml.setAttribute( "CollectionType", "Spatial" );
    break;
  case kTemporal:
    xml.setAttribute( "CollectionType", "Temporal" );
    break;
  default:
    XDM_THROW( std::invalid_argument( "Unrecognized Grid type" ) );
    break;
  }
}

std::pair< Grid*, std::size_t > CollectionGrid::findGrid( const std::size_t& elementIndex ) const {

  if ( mElementOffsets.size() != mGrids.size() ) {
    const_cast< CollectionGrid& >( *this ).updateOffsets();
  }

  std::vector< std::size_t >::const_iterator found =
    std::upper_bound( mElementOffsets.begin(), mElementOffsets.end(), elementIndex );
  assert( found != mElementOffsets.end() );
  std::size_t gridIndex = found - mElementOffsets.begin();
  std::size_t offsetIndex = elementIndex;
  if ( gridIndex > 0 ) {
    offsetIndex -= *(--found);
  }
  if ( mArrays[ gridIndex ].valid() ) {
    return std::make_pair(
      mGrids[ gridIndex ].get(),
      mArrays[ gridIndex ]->atIndex< std::size_t >( offsetIndex ) );
  } else {
    // If the array pointer is null, then we are using the complete grid.
    return std::make_pair(
      mGrids[ gridIndex ].get(),
      offsetIndex );
  }
}

void CollectionGrid::updateOffsets() {

  mElementOffsets.clear();
  for ( std::size_t arrayIndex = 0; arrayIndex < mArrays.size(); ++arrayIndex ) {
    if ( mArrays[ arrayIndex ].valid() ) {
      mElementOffsets.push_back( mArrays[ arrayIndex ]->dataspace()[0] );
    } else {
      // Special treatment if the pointer is null: this means that the whole
      // grid is being used.
      mElementOffsets.push_back( mGrids[ arrayIndex ]->numberOfElements() );
    }
    if ( arrayIndex > 0 ) {
      mElementOffsets[ arrayIndex ] += mElementOffsets[ arrayIndex - 1 ];
    }
  }
}

XDM_GRID_NAMESPACE_END

