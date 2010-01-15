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
#ifndef xdmComm_CoalescingStreamBuffer_hpp
#define xdmComm_CoalescingStreamBuffer_hpp

#include <xdm/BinaryStreamBuffer.hpp>

#include <mpi.h>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Stream buffer implementation that synchronizes data between processes in a
/// given communicator.  Processes with rank > 0 in the input communicator will
/// communicate with an MPI synchronous send while rank 0 in the communicator
/// will receive the data.
class CoalescingStreamBuffer : public xdm::BinaryStreamBuffer {
private:
  MPI_Comm mCommunicator;

public:
  /// Constructor initializes the communicator and the buffer size.
  CoalescingStreamBuffer( size_t bufSize, MPI_Comm communicator );

  virtual ~CoalescingStreamBuffer();

  /// Poll for messages from a CoalescingStreamBuffer on a remote machine.
  /// Returns true if a message is available, false otherwise.
  bool poll() const;
  
protected:
  /// Implementation of virtual sync from BinaryStreamBuffer.  Passes data
  /// between processes with send/receive depending upon process rank within an
  /// MPI communicator.
  virtual void sync();
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_CoalescingStreamBuffer_hpp

