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



namespace xdmComm {

/// Stream buffer implementation that synchronizes data between processes in a
/// given communicator.  Processes with rank > 0 in the input communicator will
/// communicate with an MPI synchronous send while rank 0 in the communicator
/// will receive the data.
///
/// This streambuf uses a buffer in memory to buffer communications between
/// processes. Messages that overrun the internal buffer will be broken into
/// messages the size of the buffer so that processes may communicate arbitrary
/// amounts of data to rank 0. Clients with knowledge of their own messaging
/// requirements can tune the buffer size to minimize communication traffic.
///
/// For example, if a client application knows that a message of 256 bytes is
/// very common in the context in which they are using the
/// CoalescingStreamBuffer, then setting a buffer size of 256 and making a
/// synchronization call only between 256 byte blocks will ensure a minimum of
/// communication traffic, as synchronization is the only call that results
/// in MPI messages being sent.
class CoalescingStreamBuffer : public xdm::BinaryStreamBuffer {
private:
  MPI_Comm mCommunicator;
  int mCurrentSource;

public:
  /// Constructor initializes the communicator and the buffer size. As described
  /// in the class documentation, clients can use knowledge of their own
  /// messaging requirements to tune the buffer size for their own system.
  /// However, all processes in the given communicator *must* use the same
  /// buffer size.
  /// @pre All processes in communicator initialize the same buffer size.
  /// @param bufSize Size of buffer to use in messaging.
  /// @param communicator MPI communicator containing all participating
  /// processes.
  CoalescingStreamBuffer( size_t bufSize, MPI_Comm communicator );

  virtual ~CoalescingStreamBuffer();

  /// Poll for messages from a CoalescingStreamBuffer on a remote machine from
  /// the given process id.
  /// @param source Rank of the process to listen for, defaults to MPI_ANY_SOURCE.
  /// @return True if a message is available, false otherwise.
  bool poll( int source = MPI_ANY_SOURCE );

  /// Query the current source for messages. This is the source that any
  /// subsequent calls to pubsync() will receive messages from on rank 0 in the
  /// communicator.
  /// @return The process that will be queried upon the next call to pubsync().
  int currentSource() const;
  
protected:

  /// Synchronize between processes. Nonzero ranks send their buffers, rank 0
  /// receives.
  /// @pre CoalescingStreamBuffer::poll has returned true to signal there is
  /// a message in the queue from a process.
  virtual int sync();

  /// In the case of overflow, synchronize the buffer to pass the contents of
  /// the buffer between processes and prepare for more data.
  virtual int overflow( int c = std::char_traits< char >::eof() );

  /// For underflow, synchronize the buffer to pass the contents of the buffer
  /// between processes and wait for more data to come in from the same source.
  virtual int uflow();
};

} // namespace xdmComm

#endif // xdmComm_CoalescingStreamBuffer_hpp

