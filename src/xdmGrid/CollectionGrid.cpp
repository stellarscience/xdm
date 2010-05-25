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

namespace xdmGrid {

CollectionGrid::CollectionGrid( CollectionType type ) :
  Grid(),
  mGrids(),
  mElementIndices(),
  mFaceEdgeIndices(),
  mElementOffsets(),
  mReferenceTypes(),
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

void CollectionGrid::appendGrid( xdm::RefPtr< Grid > grid ) {

  if ( grid.get() == this ) {
    XDM_THROW( std::invalid_argument(
      "A CollectionGrid attempted to add itself to its own collection." ) );
  }

  mGrids.push_back( grid );
  mElementIndices.push_back( xdm::RefPtr< xdm::UniformDataItem >() );
  mFaceEdgeIndices.push_back( xdm::RefPtr< xdm::UniformDataItem >() );
  mReferenceTypes.push_back( kElement );
}

void CollectionGrid::appendGrid(
  xdm::RefPtr< Grid > grid,
  xdm::RefPtr< xdm::UniformDataItem > elementIndices ) {

  // Code Review Matter (open): assert vs exception
  // Did you consider throwing an exception instead of asserting?
  // -- K. R. Walker on 2010-05-21
  
  assert( elementIndices );
  if ( elementIndices->dataspace().rank() != 1 ) {
    XDM_THROW( std::invalid_argument( "The rank of the element indices array is not 1." ) );
  }

  if ( grid.get() == this ) {
    XDM_THROW( std::invalid_argument(
      "A CollectionGrid attempted to add itself to its own collection." ) );
  }

  mGrids.push_back( grid );
  mElementIndices.push_back( elementIndices );
  mFaceEdgeIndices.push_back( xdm::RefPtr< xdm::UniformDataItem >() );
  mReferenceTypes.push_back( kElement );
}

void CollectionGrid::appendGridFaces(
  xdm::RefPtr< Grid > grid,
  xdm::RefPtr< xdm::UniformDataItem > elementIndices,
  xdm::RefPtr< xdm::UniformDataItem > faceIndices ) {

  // Code Review Matter (open): single-condition assert
  // Did you consider assert( elementIndices ); assert( faceIndices ); so that
  // at debug-time, the stringified assertion message will contain information
  // about specifically which assertion failed?
  // -- K. R. Walker on 2010-05-21
  
  assert( elementIndices && faceIndices );
  if ( elementIndices->dataspace().rank() != 1 ) {
    XDM_THROW( std::invalid_argument( "The rank of the element indices array is not 1." ) );
  }
  if ( faceIndices->dataspace().rank() != 1 ) {
    XDM_THROW( std::invalid_argument( "The rank of the face indices array is not 1." ) );
  }
  if ( elementIndices->dataspace()[0] != faceIndices->dataspace()[0] ) {
    XDM_THROW( std::invalid_argument( "The number of face indices does not match the number"
      " of element indices." ) );
  }

  if ( grid.get() == this ) {
    XDM_THROW( std::invalid_argument(
      "A CollectionGrid attempted to add itself to its own collection." ) );
  }

  mGrids.push_back( grid );
  mElementIndices.push_back( elementIndices );
  mFaceEdgeIndices.push_back( faceIndices );
  mReferenceTypes.push_back( kFace );
}

void CollectionGrid::appendGridEdges(
  xdm::RefPtr< Grid > grid,
  xdm::RefPtr< xdm::UniformDataItem > elementIndices,
  xdm::RefPtr< xdm::UniformDataItem > edgeIndices ) {

  assert( elementIndices && edgeIndices );
  if ( elementIndices->dataspace().rank() != 1 ) {
    XDM_THROW( std::invalid_argument( "The rank of the element indices array is not 1." ) );
  }
  if ( edgeIndices->dataspace().rank() != 1 ) {
    XDM_THROW( std::invalid_argument( "The rank of the edge indices array is not 1." ) );
  }
  if ( elementIndices->dataspace()[0] != edgeIndices->dataspace()[0] ) {
    XDM_THROW( std::invalid_argument( "The number of edge indices does not match the number"
      " of element indices." ) );
  }

  if ( grid.get() == this ) {
    XDM_THROW( std::invalid_argument(
      "A CollectionGrid attempted to add itself to its own collection." ) );
  }

  mGrids.push_back( grid );
  mElementIndices.push_back( elementIndices );
  mFaceEdgeIndices.push_back( edgeIndices );
  mReferenceTypes.push_back( kEdge );
}

xdm::RefPtr< Attribute > CollectionGrid::createAttribute(
  Attribute::Center center,
  Attribute::Type type,
  const std::string& name,
  xdm::primitiveType::Value dataType ) {


  // CollectionGrid element access is inherently unstructred access. The grid can reference
  // elements, faces, and edges in other grids, but not nodes.
  xdm::DataShape<> attributeSpace;

  switch( center ) {
  case Attribute::kNode:
    XDM_THROW( std::invalid_argument(
      "Node-centered attributes are not allowed for CollectionGrids." ) );
    break;
  case Attribute::kEdge:
  case Attribute::kFace:
  case Attribute::kElement:
    attributeSpace.push_back( numberOfElements() );
    break;
  case Attribute::kGrid:
    // no-op because there is only one value for the whole grid.
    break;
  default:
    XDM_THROW( std::invalid_argument( "An unknown attribute centering was specified." ) );
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

  // Create the data item for the attribute values.
  xdm::RefPtr< xdm::UniformDataItem > attributeDataItem(
    new xdm::UniformDataItem( dataType, attributeSpace ) );

  // Create the attribute.
  xdm::RefPtr< Attribute > attribute( new Attribute( type, center ) );
  attribute->setName( name );
  attribute->setDataItem( attributeDataItem );

  addAttribute( attribute );

  return attribute;
}

std::size_t CollectionGrid::numberOfElements() const {
  std::size_t elementCount = 0;
  for ( std::size_t i = 0; i < mElementIndices.size(); ++i ) {
    if ( mElementIndices[i] ) {
      elementCount += mElementIndices[i]->dataspace()[0];
    } else {
      // If the array does not exist, then we assume that the whole grid is being used.
      elementCount += mGrids[i]->numberOfElements();
    }
  }
  return elementCount;
}

Element CollectionGrid::element( const std::size_t& elementIndex ) const
{
  std::pair< std::size_t, std::size_t > found = findGrid( elementIndex );

  // Special treatment for null element index list: assume we are referencing the whole
  // grid.
  std::size_t element = found.second;
  if ( mElementIndices[ found.first ] ) {
    element = mElementIndices[ found.first ]->atIndex< std::size_t >( found.second );
  }

  switch( mReferenceTypes[ found.first ] ) {
  case kFace:
    return mGrids[ found.first ]->element( element ).face(
      mFaceEdgeIndices[ found.first ]->atIndex< std::size_t >( found.second ) );
  case kEdge:
    return mGrids[ found.first ]->element( element ).edge(
      mFaceEdgeIndices[ found.first ]->atIndex< std::size_t >( found.second ) );
   default:
    return mGrids[ found.first ]->element( element );
  }
}

void CollectionGrid::traverse( xdm::ItemVisitor& iv ) {
  Grid::traverse( iv );

  std::for_each( mGrids.begin(), mGrids.end(), xdm::ApplyVisitor( iv ) );
  for ( std::size_t i = 0; i < mGrids.size(); ++i ) {
    if ( mElementIndices[i] ) {
      mElementIndices[i]->accept( iv );
    }
    if ( mFaceEdgeIndices[i] ) {
      mFaceEdgeIndices[i]->accept( iv );
    }
  }
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

// Finds the grid index and element offset index.
std::pair< std::size_t, std::size_t > CollectionGrid::findGrid( const std::size_t& elementIndex ) const {

  if ( mElementOffsets.size() != mGrids.size() ) {
    const_cast< CollectionGrid& >( *this ).updateOffsets();
  }

  std::vector< std::size_t >::const_iterator found =
    std::upper_bound( mElementOffsets.begin(), mElementOffsets.end(), elementIndex );
  // Code Review Matter (open): assert vs exception
  // Is it possible for a data driven process to cause an invalid elementIndex to be
  // supplied to this method? Would an exception be more appropriate?
  // Even if triggering this event were only possible via a programming error, wouldn't
  // an exectption and smoother application shutdown be destired over an assert crash?
  // -- Todd on 2010-05-21
  assert( found != mElementOffsets.end() );
  std::size_t gridIndex = found - mElementOffsets.begin();
  std::size_t offsetIndex = elementIndex;
  if ( gridIndex > 0 ) {
    offsetIndex -= *(--found);
  }
  return std::make_pair(
    gridIndex,
    offsetIndex );
}

void CollectionGrid::updateOffsets()
{
  // Code Review Matter (open): Code comments
  // I don't quite follow the logic of this method, could it use more comments?
  // -- Todd on 2010-05-21

  mElementOffsets.clear();
  for ( std::size_t arrayIndex = 0; arrayIndex < mElementIndices.size(); ++arrayIndex ) {
    if ( mElementIndices[ arrayIndex ] ) {
      mElementOffsets.push_back( mElementIndices[ arrayIndex ]->dataspace()[0] );
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

} // namespace xdmGrid

