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

#include <xdm/Dataset.hpp>

#include <mpi.h>

#include <memory>
#include <vector>

#include <xdmComm/NamespaceMacro.hpp>

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
class MpiDatasetProxy : public xdm::Dataset {
public:
  /// Construct a proxy for the input dataset.
  /// @param communicator Communicator with relevant processes.
  /// @param dataset The actual dataset that will handle writing.
  /// @param bufSizeHint Suggested size for communication buffer.
  // Code Review Matter (open): Naming conventions.
  // Did you consider not abbreviating "buffer" in the parameter name below?
  // Will Dicharry 2010-01-19
  MpiDatasetProxy( 
    MPI_Comm communicator, 
    xdm::Dataset* dataset,
    size_t bufSizeHint );

  virtual ~MpiDatasetProxy();

  /// Update call to update myself and the real dataset.
  virtual void update();

  /// Pass-through to get the format of the underlying dataset.
  virtual const char* format();
  /// Pass-through to get the real dataset to write it's text content.
  virtual void writeTextContent( xdm::XmlTextContent& text );

protected:
  /// Initialization calls underlying dataset initialization only if this
  /// process is rank 0 within the communicator.
  virtual void initializeImplementation( 
    xdm::primitiveType::Value type,
    const xdm::DataShape<>& shape );

  /// Serialization process packs and sends the array and selection data to rank
  /// 0 in the communicator for writing.
  virtual void serializeImplementation( 
    const xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );

  /// Finalization calls underlying dataset finalization only if this process is
  /// rank 0 in the assigned communicator.
  virtual void finalizeImplementation();

private:
  MPI_Comm mCommunicator;
  xdm::RefPtr< xdm::Dataset > mDataset;
  std::auto_ptr< xdmComm::CoalescingStreamBuffer > mCommBuffer;
  xdm::RefPtr< xdm::ByteArray > mArrayBuffer;
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_MpiDatasetProxy_hpp

