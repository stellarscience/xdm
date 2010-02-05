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
#ifndef xdmComm_MpiDatasetProxy_hpp
#define xdmComm_MpiDatasetProxy_hpp

#include <xdm/ProxyDataset.hpp>

#include <mpi.h>

#include <memory>
#include <vector>

#include <xdmComm/NamespaceMacro.hpp>

// Code Review Matter (open): Merging forward declarations
// Did you consider merging these forward declarations since they are in the same namespace? 
// -- Birch Polega on 2009-01-18

namespace xdm {
  class ByteArray;
}

namespace xdm {
  class StructuredArray;
}

XDM_COMM_NAMESPACE_BEGIN

class CoalescingStreamBuffer;

/// Dataset proxy that uses MPI to communicate data between processes before
/// actually writing the data to a dataset.  The communication procedures assume
/// that the cluster is heterogeneous and the objects to be sent are bitwise
/// serializable.
///
/// This class uses a CoalescingStreamBuffer to buffer interprocess
/// communications. The constructor for this class takes a hint to control the
/// size of the buffer used for communications. A poor choice for this size can
/// greatly impact application performance. Using a buffer size that is too
/// small can result in messages being overly buffered and therefore increase
/// communication traffic. Clients with knowledge of their own array sizes can
/// tune this parameter to ensure a minimum of communication is required when
/// passing arrays with dataset contents between processes.
class MpiDatasetProxy : public xdm::ProxyDataset {
public:
  // Code Review Matter (open): Naming conventions.
  // Did you consider not abbreviating "buffer" in the parameter name below?
  // Will Dicharry 2010-01-19

  /// Construct a proxy for the input dataset. As described in the class
  /// documentation, a poor choice for the buffer size hint can greatly impact
  /// application performance.
  /// @see MpiDatasetProxy
  /// @see CoalescingStreamBuffer
  /// @pre All processes in the communicator must use the same buffer size.
  /// @param communicator Communicator with relevant processes.
  /// @param dataset The actual dataset that will handle writing.
  /// @param bufSizeHint Suggested size for communication buffer.
  MpiDatasetProxy( 
    MPI_Comm communicator, 
    xdm::RefPtr< xdm::Dataset > dataset,
    size_t bufSizeHint );

  virtual ~MpiDatasetProxy();

protected:
  /// Initialization calls underlying dataset initialization only if this
  /// process is rank 0 within the communicator.
  virtual void initializeImplementation( 
    xdm::primitiveType::Value type,
    const xdm::DataShape<>& shape,
    const xdm::Dataset::InitializeMode& mode );

  /// Serialization process packs and sends the array and selection data to rank
  /// 0 in the communicator for writing.
  virtual void serializeImplementation( 
    const xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );

  /// Deserialization will read the entire dataset into the output array. No
  /// communication is performed.
  virtual void deserializeImplementation(
    xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );

  /// Finalization calls underlying dataset finalization only if this process is
  /// rank 0 in the assigned communicator.
  virtual void finalizeImplementation();

private:
  MPI_Comm mCommunicator;
  std::auto_ptr< xdmComm::CoalescingStreamBuffer > mCommBuffer;
  xdm::RefPtr< xdm::ByteArray > mArrayBuffer;
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_MpiDatasetProxy_hpp

