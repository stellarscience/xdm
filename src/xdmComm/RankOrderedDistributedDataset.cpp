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
#include <xdmComm/RankOrderedDistributedDataset.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/CoordinateDataSelection.hpp>
#include <xdm/DataSelection.hpp>
#include <xdm/DataSelectionVisitor.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/StaticAssert.hpp>

#include <algorithm>

#include <climits>

XDM_COMM_NAMESPACE_BEGIN

namespace {
  // SelectionVisitor that turns the different selection types into coordinate
  // or hyperslab selections with an offset in the first dimension.
  class OffsetSelectionVisitor : public xdm::DataSelectionVisitor {
  public:
    OffsetSelectionVisitor(
      xdm::DataShape<>::size_type offset,
      const xdm::DataShape<>& shape ) :
      xdm::DataSelectionVisitor(),
      mOffset( offset ),
      mShape( shape ) {
    }

    virtual ~OffsetSelectionVisitor() {}

    // An AllDataSelection is converted to a hyperslab data selection with a
    // start of offset in the first dimension, 0 in all others, a stride of 1 in
    // all dimensions, and a count to match the number of elements in all
    // dimensions.
    virtual void apply( const xdm::AllDataSelection& ) {
      xdm::HyperSlab<> resultSlab( mShape );
      xdm::HyperSlab<>::iterator start = resultSlab.beginStart();
      *start = mOffset;
      std::fill( ++start, resultSlab.endStart(), 0 );
      std::fill( resultSlab.beginStride(), resultSlab.endStride(), 1 );
      std::copy( mShape.begin(), mShape.end(), resultSlab.beginCount() );
      mResult = xdm::makeRefPtr( new xdm::HyperslabDataSelection( resultSlab ) );
    }

    // A HyperslabDataSelection is left alone, except the start location in the
    // first dimension is set to offset.
    virtual void apply( const xdm::HyperslabDataSelection& selection ) {
      xdm::HyperSlab<> resultSlab = selection.hyperslab();
      resultSlab.setStart( 0, mOffset );
      mResult = xdm::makeRefPtr( new xdm::HyperslabDataSelection( resultSlab ) );
    }

/*
    // A coordinate data selection gets the offset added to the first component
    // of each vertex.
    virtual void apply( const xdm::CoordinateDataSelection& selection ) {
      // make a shallow copy of the coordinate values.
      xdm::CoordinateArray<> resultCoordinates(
        const_cast< xdm::CoordinateArray<>& >( selection.coordinates() ) );
      size_t * begin = resultCoordinates.values();
      size_t * end = begin + resultCoordinates.numberOfElements();
      for ( size_t * p = begin; p < end; p += resultCoordinates.rank() ) {
        *p += mOffset;
      }
      mResult = xdm::makeRefPtr( new xdm::CoordinateDataSelection( resultCoordinates ) );
    }
*/

    xdm::RefPtr< xdm::DataSelection > result() { return mResult; }

  private:
    xdm::DataShape<>::size_type mOffset;
    xdm::DataShape<> mShape;
    xdm::RefPtr< xdm::DataSelection > mResult;
  };
} // namespace

RankOrderedDistributedDataset::RankOrderedDistributedDataset(
  xdm::RefPtr< xdm::Dataset > dataset,
  MPI_Comm communicator ) :
  xdm::ProxyDataset( dataset ),
  mCommunicator( communicator ),
  mStartLocation( 0 ),
  mDataShape() {
}

RankOrderedDistributedDataset::~RankOrderedDistributedDataset() {
}

xdm::DataShape<> RankOrderedDistributedDataset::initializeImplementation(
  xdm::primitiveType::Value type,
  const xdm::DataShape<> &shape,
  const xdm::Dataset::InitializeMode &mode )
{
  int processes;
  MPI_Comm_size( mCommunicator, &processes );
  int rank;
  MPI_Comm_rank( mCommunicator, &rank );

  // save the original shape for use later.
  mDataShape = shape;

  // allocate a working buffer for the communication.
  typedef std::vector< xdm::DataShape<>::size_type > SizeVector;
  SizeVector commBuffer( processes, 0 );

  // The gather and scatter calls below assume that a byte and a char are the
  // same size. Fail to compile if they aren't.
  XDM_STATIC_ASSERT( CHAR_BIT == 8 );

  // Gather the first dimension from all participating processes.
  mStartLocation = shape[0];
  MPI_Gather(
    &mStartLocation,
    sizeof( xdm::DataShape<>::size_type ),
    MPI_BYTE,
    &commBuffer[0],
    sizeof( xdm::DataShape<>::size_type ),
    MPI_BYTE,
    0,
    mCommunicator );

  // Rank 0 now has the start location for all processes. Compute a running sum
  // to determine each process' starting location from those before it.
  xdm::DataShape<>::size_type sum( 0 );
  for ( SizeVector::iterator p = commBuffer.begin(); p != commBuffer.end(); ++p ) {
    xdm::DataShape<>::size_type tmp = *p;
    *p = sum;
    sum += tmp;
  }

  // Now the comm buffer has the start location for each process at the index
  // specified by the process rank. Scatter back to all processes and store
  // the result for this process in the start location member variable for use
  // during serialization.
  MPI_Scatter(
    &commBuffer[0],
    sizeof( xdm::DataShape<>::size_type ),
    MPI_BYTE,
    &mStartLocation,
    sizeof( xdm::DataShape<>::size_type ),
    MPI_BYTE,
    0,
    mCommunicator );

  // Rank 0 is the only one to contain the complete sum, broadcast this
  // information to everyone else so they can call initialize with the correct
  // dimensions.
  MPI_Bcast(
    &sum,
    sizeof( xdm::DataShape<>::size_type ),
    MPI_BYTE,
    0,
    mCommunicator );
  // initialize with modified dimensions.
  xdm::DataShape<> expandedBounds( shape );
  expandedBounds[0] = sum;
  return xdm::ProxyDataset::initializeImplementation( type, expandedBounds, mode );
}

void RankOrderedDistributedDataset::serializeImplementation(
  const xdm::StructuredArray *data,
  const xdm::DataSelectionMap &selectionMap )
{
  // apply an offset to the data on disk, the range of the selectionMap.
  OffsetSelectionVisitor offsetSelection( mStartLocation, mDataShape );
  selectionMap.range()->accept( offsetSelection );

  xdm::DataSelectionMap newSelectionMap( selectionMap );
  newSelectionMap.setRange( offsetSelection.result() );
  xdm::ProxyDataset::serializeImplementation( data, newSelectionMap );
}

XDM_COMM_NAMESPACE_END
