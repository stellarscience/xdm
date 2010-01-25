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
#include <xdmComm/DistributedItemCollectionProxy.hpp>

#include <xdmComm/BarrierOnExit.hpp>

#include <xdm/BinaryIOStream.hpp>
#include <xdm/BinaryStreamOperations.hpp>
#include <xdm/CompositeDataItem.hpp>
#include <xdm/DataItem.hpp>
#include <xdm/ItemVisitor.hpp>
#include <xdm/UniformDataItem.hpp>

XDM_COMM_NAMESPACE_BEGIN

namespace {

class VisitorWrapper : public xdm::ItemVisitor {
public:
  VisitorWrapper( ItemVisitor& iv, xdm::BinaryOStream& ostr ) :
    mWrappedVisitor( iv ),
    mOStr( ostr ) {
  }

  virtual ~VisitorWrapper() {
  }

  virtual void apply( xdm::Item& item ) {
    resetApplyAndCommunicate( item );
  }
  virtual void apply( xdm::DataItem& item ) {
    resetApplyAndCommunicate( item );
  }
  virtual void apply( xdm::CompositeDataItem& item ) {
    resetApplyAndCommunicate( item );
  }
  virtual void apply( xdm::UniformDataItem& item ) {
    resetApplyAndCommunicate( item );
  }

private:
  xdm::ItemVisitor & mWrappedVisitor;
  xdm::BinaryOStream & mOStr;

  template< typename ItemT >
  void resetApplyAndCommunicate( ItemT & item ) {
    // reset and prepare to traverse the item
    mWrappedVisitor.reset();

    // apply the wrapped visitor to the item.
    item.accept( mWrappedVisitor );

    // capture the state and stream it
    mWrappedVisitor.captureState( mOStr );
    mOStr << xdm::flush;
  }
};

} // namespace

DistributedItemCollectionProxy::DistributedItemCollectionProxy(
  xdm::Item* item, MPI_Comm communicator, size_t bufferSizeHint ) :
  mItem( item ),
  mCommunicator( communicator ),
  mCommBuffer( new xdmComm::CoalescingStreamBuffer( bufferSizeHint, communicator ) ) {
}

DistributedItemCollectionProxy::~DistributedItemCollectionProxy() {
}

void DistributedItemCollectionProxy::accept( xdm::ItemVisitor& iv ) {

  // block on exit so that messages don't get reordered.
  BarrierOnExit barrier( mCommunicator );

  int processes;
  MPI_Comm_size( mCommunicator, &processes );
  int rank;
  MPI_Comm_rank( mCommunicator, &rank );

  // Rank 0 applies the visitor to the local element and waits for all processes
  // to report their visitation results. Others explicitly traverse their local
  // subtrees and send the result back to 0 for accumulation.
  if ( rank == 0 ) {

    // Apply the visitor to the wrapped Item.
    mItem->accept( iv );

    // Wait until we visit all processes participating in the communicator.
    int processCount = 1;
    while ( processCount < processes ) {

      // Poll for messages from other processes.
      while ( mCommBuffer->poll() ) {
        // A process is reporting. Update the visitor state to accumulate the
        // results from the distributed Item.
        xdm::BinaryIStream input( mCommBuffer.get() );
        input.sync();
        iv.restoreState( input );
        processCount++;
      }

    }

  } else {
    // create a stream for the children of this object to write to
    xdm::BinaryOStream output( mCommBuffer.get() );
    // Wrap the visitor so that the results of each child are reported
    // individually.
    VisitorWrapper wrapper( iv, output );
    traverse( wrapper );
  }
}

void DistributedItemCollectionProxy::traverse( xdm::ItemVisitor& iv ) {
  // forward to wrapped Item
  mItem->traverse( iv );
}

void DistributedItemCollectionProxy::update() {
  // update this
  xdm::Item::update();
  // update the wrapped object
  mItem->update();
}

void DistributedItemCollectionProxy::writeMetadata(
  xdm::XmlMetadataWrapper& metadata ) {
  // write the metadata for the wrapped Item first
  mItem->writeMetadata( metadata );
}

XDM_COMM_NAMESPACE_END
