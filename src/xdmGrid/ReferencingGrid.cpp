//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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
#include <xdmGrid/ReferencingGrid.hpp>

#include <xdmGrid/Element.hpp>
#include <xdmGrid/Topology.hpp>

#include <xdm/DataShape.hpp>
#include <xdm/ItemVisitor.hpp>
#include <xdm/ThrowMacro.hpp>
#include <xdm/UniformDataItem.hpp>

#include <algorithm>
#include <stdexcept>
#include <utility>

XDM_GRID_NAMESPACE_BEGIN

// A helper class for accessing Elements via indirection into UniformGrids.
class ReferencingGrid::MultiReferenceElementImp : public xdmGrid::ElementSharedConnectivityLookup {
public:
  MultiReferenceElementImp( ReferencingGrid& grid ) : mGrid( grid ) {
  }

  virtual std::size_t at( const std::size_t& elementIndex, const std::size_t& nodeIndex ) const {
    std::pair< UniformGrid*, std::size_t > found = mGrid.findGrid( elementIndex );
    return found.first->topology()->elementConnections( found.second )[ nodeIndex ];
  }

  virtual const Geometry& geometry( const std::size_t& elementIndex ) const {
    std::pair< UniformGrid*, std::size_t > found = mGrid.findGrid( elementIndex );
    return *found.first->geometry();
  }

private:
  ReferencingGrid& mGrid;
};

ReferencingGrid::ReferencingGrid() {
  setElementSharedImp( xdm::makeRefPtr( new MultiReferenceElementImp( *this ) ) );
}

ReferencingGrid::~ReferencingGrid() {
}

std::size_t ReferencingGrid::numberOfElements() const {

  std::size_t elementCount = 0;
  for ( std::size_t i = 0; i < mArrays.size(); ++i ) {
    if ( mArrays[i] ) {
      elementCount += mArrays[i]->dataspace()[0];
    } else {
      // If the array does not exist, then we assume that the whole grid is being used.
      elementCount += mGrids[i]->topology()->numberOfElements();
    }
  }
  return elementCount;
}

void ReferencingGrid::setNumberOfReferencedGrids( const std::size_t n ) {
  mGrids.resize( n );
  mArrays.resize( n );
}

void ReferencingGrid::appendReferenceGrid(
  xdm::RefPtr< UniformGrid > grid,
  xdm::RefPtr< xdm::UniformDataItem > elementIndices ) {

  // A grid should NEVER reference itself.
  assert( grid.get() != this );

  mGrids.push_back( grid );
  mArrays.push_back( elementIndices );
}

void ReferencingGrid::setReferenceGrid(
  const std::size_t gridIndex,
  xdm::RefPtr< UniformGrid > grid,
  xdm::RefPtr< xdm::UniformDataItem > elementIndices ) {

  // A grid should NEVER reference itself.
  assert( grid.get() != this );

  assert( gridIndex < mGrids.size() );

  mGrids[ gridIndex ] = grid;
  mArrays[ gridIndex ] = elementIndices;
}

xdm::RefPtr< const ElementTopology > ReferencingGrid::elementTopology(
  const std::size_t& elementIndex ) const {

  std::pair< UniformGrid*, std::size_t > found = findGrid( elementIndex );
  return found.first->elementTopology( found.second );
}

void ReferencingGrid::traverse( xdm::ItemVisitor& iv ) {
  Grid::traverse( iv );

  // The visitor does not get applied to the grid children because ReferencingGrid
  // does not own them. However, it does own the index data sets.
  std::for_each( mArrays.begin(), mArrays.end(), xdm::ApplyVisitor( iv ) );
}

void ReferencingGrid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Grid::writeMetadata( xml );

  // write grid type
  xml.setAttribute( "GridType", "Referencing" );
}

std::pair< UniformGrid*, std::size_t >
ReferencingGrid::findGrid( const std::size_t& elementIndex ) const {

  if ( mElementOffsets.size() != mGrids.size() ) {
    const_cast< ReferencingGrid& >( *this ).updateOffsets();
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

void ReferencingGrid::updateOffsets() {

  mElementOffsets.clear();
  for ( std::size_t arrayIndex = 0; arrayIndex < mArrays.size(); ++arrayIndex ) {
    if ( mArrays[ arrayIndex ].valid() ) {
      mElementOffsets.push_back( mArrays[ arrayIndex ]->dataspace()[0] );
    } else {
      // Special treatment if the pointer is null: this means that the whole
      // grid is being used.
      mElementOffsets.push_back( mGrids[ arrayIndex ]->topology()->numberOfElements() );
    }
    if ( arrayIndex > 0 ) {
      mElementOffsets[ arrayIndex ] += mElementOffsets[ arrayIndex - 1 ];
    }
  }
}

XDM_GRID_NAMESPACE_END

