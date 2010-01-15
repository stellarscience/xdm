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

#include <xdmComm/BinaryIOStream.hpp>
#include <xdmComm/BinaryStreamOperations.hpp>

#include <xdm/CollectMetadataOperation.hpp>

XDM_COMM_NAMESPACE_BEGIN

DistributedItemCollectionProxy::DistributedItemCollectionProxy(
  xdm::Item* item, MPI_Comm communicator, size_t bufferSizeHint ) :
  mItem( item ),
  mCommunicator( communicator ),
  mCommBuffer( new xdmComm::CoalescingStreamBuffer( bufferSizeHint, communicator ) ) {
}

DistributedItemCollectionProxy::~DistributedItemCollectionProxy() {
}

void DistributedItemCollectionProxy::accept( xdm::ItemVisitor& iv ) {
  // forward to wrapped Item
  mItem->accept( iv );
  iv.apply( *this );
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

  int processCount;
  MPI_Comm_size( mCommunicator, &processCount );
  int rank;
  MPI_Comm_rank( mCommunicator, &rank );
  if ( rank == 0 ) {

    int processesCompleted = 1;
    while ( processesCompleted < processCount ) {
      while( mCommBuffer->poll() ) {
        BinaryIStream istr( mCommBuffer.get() );
        istr.sync();
        xdm::RefPtr< xdm::XmlObject > xml( new xdm::XmlObject );
        istr >> *xml;
        metadata.completeObject()->appendChild( xml );
        processesCompleted++;
      }
    }

  } else {

    BinaryOStream ostr( mCommBuffer.get() );

    xdm::RefPtr< xdm::XmlObject > xml( metadata.completeObject() );
    xdm::CollectMetadataOperation collector( xml );
    mItem->accept( collector );
    ostr << *xml << flush;

  }

}

XDM_COMM_NAMESPACE_END
