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

#include <xdmGrid/Cell.hpp>
#include <xdmGrid/Topology.hpp>

#include <xdm/ItemVisitor.hpp>
#include <xdm/ThrowMacro.hpp>
#include <xdm/UniformDataItem.hpp>

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace {

// A helper class for accessing cells via indirection into UniformGrids.
class MultiReferenceCellImp : public xdmGrid::CellSharedImp {
public:
  MultiReferenceCellImp(
    std::vector< xdm::RefPtr< xdmGrid::UniformGrid > >& grids,
    std::vector< xdm::RefPtr< xdm::UniformDataItem > >& arrays ) :
      mGrids( grids ), mArrays( arrays ), mCellOffsets() {
  }

  virtual xdmGrid::ConstNode node( std::size_t cellIndex, std::size_t nodeIndex ) const {
    std::pair< xdmGrid::UniformGrid*, std::size_t > found = findGrid( cellIndex );
    return found.first->geometry()->node(
      found.first->topology()->cellConnections( found.second )[ nodeIndex ] );
  }

  virtual xdmGrid::CellType::Type cellType( std::size_t cellIndex ) const {
    std::pair< xdmGrid::UniformGrid*, std::size_t > found = findGrid( cellIndex );
    return found.first->topology()->cellType( found.second );
  }

private:
  std::pair< xdmGrid::UniformGrid*, std::size_t > findGrid( std::size_t cellIndex ) const {
    if ( mCellOffsets.size() != mGrids.size() ) {
      computeOffsets();
    }
    std::vector< std::size_t >::const_iterator found =
      std::upper_bound( mCellOffsets.begin(), mCellOffsets.end(), cellIndex );
    assert( found != mCellOffsets.end() );
    std::size_t gridIndex = found - mCellOffsets.begin();
    std::size_t offsetIndex = cellIndex;
    if ( gridIndex > 0 ) {
      offsetIndex -= *(--found);
    }
    if ( mArrays[ gridIndex ].valid() ) {
      return std::make_pair(
        mGrids[ gridIndex ].get(),
        (*mArrays[ gridIndex ]->typedArray< std::size_t >())[ offsetIndex ] );
    } else {
      // If the array pointer is null, then we are using the complete grid.
      return std::make_pair(
        mGrids[ gridIndex ].get(),
        offsetIndex );
    }
  }

  void computeOffsets() const {
    assert( mGrids.size() == mArrays.size() );

    mCellOffsets.clear();
    for ( std::size_t arrayIndex = 0; arrayIndex < mArrays.size(); ++arrayIndex ) {
      if ( mArrays[ arrayIndex ].valid() ) {
        mCellOffsets.push_back( mArrays[ arrayIndex ]->dataspace()[0] );
      } else {
        // Special treatment if the pointer is null: this means that the whole
        // grid is being used.
        mCellOffsets.push_back( mGrids[ arrayIndex ]->topology()->numberOfCells() );
      }
      if ( arrayIndex > 0 ) {
        mCellOffsets[ arrayIndex ] += mCellOffsets[ arrayIndex - 1 ];
      }
    }
  }

  std::vector< xdm::RefPtr< xdmGrid::UniformGrid > >& mGrids;
  std::vector< xdm::RefPtr< xdm::UniformDataItem > >& mArrays;
  mutable std::vector< std::size_t > mCellOffsets;
};

} // anon namespace

XDM_GRID_NAMESPACE_BEGIN

ReferencingGrid::ReferencingGrid() {
  setCellSharedImp( xdm::makeRefPtr(
    new MultiReferenceCellImp(
      xdm::ObjectCompositionMixin< UniformGrid >::allChildren(),
      xdm::ObjectCompositionMixin< xdm::UniformDataItem >::allChildren() ) ) );
}

ReferencingGrid::~ReferencingGrid() {
}

std::size_t ReferencingGrid::numberOfCells() const {
  std::size_t numberOfDataItems =
    xdm::ObjectCompositionMixin< xdm::UniformDataItem >::numberOfChildren();
  assert( numberOfDataItems ==
    xdm::ObjectCompositionMixin< xdmGrid::UniformGrid >::numberOfChildren() );

  std::size_t cellCount = 0;
  for ( std::size_t i = 0; i < numberOfDataItems; ++i )     {
    xdm::RefPtr< const xdm::UniformDataItem > array = xdm::child< xdm::UniformDataItem >( *this, i );
    if ( array.valid() ) {
      cellCount += array->typedArray< std::size_t >()->size();
    } else {
      // If the array does not exist, then we assume that the whole grid is being used.
      xdm::RefPtr< const xdmGrid::UniformGrid > grid = xdm::child< xdmGrid::UniformGrid >( *this, i );
      cellCount += grid->topology()->numberOfCells();
    }
  }
  return cellCount;
}

void ReferencingGrid::setNumberOfReferencedGrids( const std::size_t n ) {
  xdm::ObjectCompositionMixin< UniformGrid >::setNumberOfChildren( n );
  xdm::ObjectCompositionMixin< xdm::UniformDataItem >::setNumberOfChildren( n );
}

void ReferencingGrid::appendReferenceGrid(
  xdm::RefPtr< UniformGrid > grid,
  xdm::RefPtr< xdm::UniformDataItem > cellIndices ) {

  xdm::appendChild< UniformGrid >( *this, grid );
  xdm::appendChild< xdm::UniformDataItem >( *this, cellIndices );
}

void ReferencingGrid::setReferenceGrid(
  const std::size_t gridIndex,
  xdm::RefPtr< UniformGrid > grid,
  xdm::RefPtr< xdm::UniformDataItem > cellIndices ) {

  assert( gridIndex < xdm::ObjectCompositionMixin< UniformGrid >::numberOfChildren() );

  xdm::setChild< UniformGrid >( *this, gridIndex, grid );
  xdm::setChild< xdm::UniformDataItem >( *this, gridIndex, cellIndices );
}

void ReferencingGrid::traverse( xdm::ItemVisitor& iv ) {
  Grid::traverse( iv );

  // The visitor does not get applied to the grid children because ReferencingGrid
  // does not own them. However, it does own the index data sets.
  std::for_each(
    xdm::begin< xdm::UniformDataItem >( *this ),
    xdm::end< xdm::UniformDataItem >( *this ),
    xdm::ApplyVisitor( iv ) );
}

void ReferencingGrid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Grid::writeMetadata( xml );

  // write grid type
  xml.setAttribute( "GridType", "Referencing" );
}

XDM_GRID_NAMESPACE_END

