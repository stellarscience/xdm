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
#ifndef xdmComm_DistributedItemCollectionProxy_hpp
#define xdmComm_DistributedItemCollectionProxy_hpp

#include <xdmComm/CoalescingStreamBuffer.hpp>

#include <xdm/Item.hpp>

#include <mpi.h>

#include <memory>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Proxy object to represent a collection of items in a distributed
/// environment. This is a proxy class that wraps around any xdm Item that may
/// contain children. The proxy allows the children of an item to reside on
/// another machine in a distributed environment, but defines traversal so that
/// children of the corresponding node on another process are visible from
/// rank 0 in the specified communicator.
class DistributedItemCollectionProxy : public xdm::Item {
public:

  /// Construct with an item to wrap and an MPI communicator to use when
  /// collecting data.
  /// @param item The xdm::Item to act as a proxy for.
  /// @param communicator Communicator containing participating processes.
  DistributedItemCollectionProxy(
    xdm::Item* item,
    MPI_Comm communicator,
    size_t bufferSizeHint );

  /// Destructor does not free the communicator.
  virtual ~DistributedItemCollectionProxy();

  //-- xdm::Item Implementations --//

  /// Visitor accept forwards to the wrapped Item.
  virtual void accept( xdm::ItemVisitor& iv );
  /// Visitor traversal traverses local children and corresponding distributed
  /// children.
  virtual void traverse( xdm::ItemVisitor& iv );
  /// Write the metadata for the collection Item and collect the metadata for
  /// the remote children. This method will write the metadata for the wrapped
  /// item, and then use MPI to collect the complete XmlObject representation
  /// of the distributed children.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& metadata );

private:
  xdm::RefPtr< xdm::Item > mItem;
  MPI_Comm mCommunicator;
  std::auto_ptr< xdmComm::CoalescingStreamBuffer > mCommBuffer;
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_DistributedItemCollectionProxy_hpp
