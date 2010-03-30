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
    std::vector< xdm::RefPtr< xdm::UniformDataItem > >& arrays,
    std::vector< std::size_t >& offsets ) :
      mGrids( grids ), mArrays( arrays ), mOffsets( offsets ) {
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
    std::vector< std::size_t >::const_iterator found =
      std::upper_bound( mOffsets.begin(), mOffsets.end(), cellIndex );
    assert( found != mOffsets.end() );
    std::size_t gridIndex = found - mOffsets.begin();
    std::size_t offsetIndex = cellIndex;
    if ( gridIndex > 0 ) {
      offsetIndex -= *(--found);
    }
    return std::make_pair(
      mGrids[ gridIndex ].get(),
      (*mArrays[ gridIndex ]->typedArray< std::size_t >())[ offsetIndex ] );
  }

  std::vector< xdm::RefPtr< xdmGrid::UniformGrid > >& mGrids;
  std::vector< xdm::RefPtr< xdm::UniformDataItem > >& mArrays;
  std::vector< std::size_t >& mOffsets;
};

} // anon namespace

XDM_GRID_NAMESPACE_BEGIN

ReferencingGrid::ReferencingGrid() {
  setCellSharedImp( xdm::makeRefPtr(
    new MultiReferenceCellImp(
      xdm::ObjectCompositionMixin< UniformGrid >::allChildren(),
      xdm::ObjectCompositionMixin< xdm::UniformDataItem >::allChildren(),
      mCellOffsets ) ) );
}

ReferencingGrid::~ReferencingGrid() {
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
  computeOffsets();
}

void ReferencingGrid::setReferenceGrid(
  xdm::RefPtr< UniformGrid > grid,
  xdm::RefPtr< xdm::UniformDataItem > cellIndices,
  const std::size_t gridIndex ) {

  assert( gridIndex < xdm::ObjectCompositionMixin< UniformGrid >::numberOfChildren() );

  xdm::setChild< UniformGrid >( *this, gridIndex, grid );
  xdm::setChild< xdm::UniformDataItem >( *this, gridIndex, cellIndices );
  computeOffsets();
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

void ReferencingGrid::computeOffsets() {
  assert( xdm::ObjectCompositionMixin< UniformGrid >::numberOfChildren() ==
          xdm::ObjectCompositionMixin< xdm::UniformDataItem >::numberOfChildren() );

  mCellOffsets.clear();
  std::size_t numberOfArrays =
    xdm::ObjectCompositionMixin< xdm::UniformDataItem >::numberOfChildren();
  for ( std::size_t childIndex = 0; childIndex < numberOfArrays; ++childIndex ) {
    xdm::RefPtr< xdm::UniformDataItem > item =
      xdm::child< xdm::UniformDataItem >( *this, childIndex );
    if ( item.valid() ) {
      mCellOffsets.push_back( item->dataspace()[0] );
    } else {
      mCellOffsets.push_back( 0 );
    }
    if ( childIndex > 0 ) {
      mCellOffsets[ childIndex ] += mCellOffsets[ childIndex - 1 ];
    }
  }
}

XDM_GRID_NAMESPACE_END

