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
#include <xdmComm/CoalescingStreamBuffer.hpp>
#include <xdmComm/MpiMessageTag.hpp>

namespace xdmComm {

CoalescingStreamBuffer::CoalescingStreamBuffer(
  size_t bufSize,
  MPI_Comm communicator ) :
  xdm::BinaryStreamBuffer( bufSize ),
  mCommunicator( communicator ) {
}

CoalescingStreamBuffer::~CoalescingStreamBuffer() {
}

bool CoalescingStreamBuffer::poll( int source ) {
  MPI_Status status;
  int flag;

  MPI_Iprobe( 
    source,
    MpiMessageTag::kWriteData, 
    mCommunicator,
    &flag,
    &status );

  if ( flag != 0 ) {
    // a message was found.
    mCurrentSource = status.MPI_SOURCE;
    return true;
  } else {
    return false;
  }
}

int CoalescingStreamBuffer::currentSource() const {
  return mCurrentSource;
}

int CoalescingStreamBuffer::sync() {
  // get the process rank to decide what to do
  int localRank;
  MPI_Comm_rank( mCommunicator, &localRank );

  if ( localRank != 0 ) {
    // non-zero ranks send to rank 0
    MPI_Ssend( 
      bufferStart(),
      bufferSize(),
      MPI_BYTE, 
      0, 
      MpiMessageTag::kWriteData, 
      mCommunicator );
  } else {
    // rank 0 receives.
    MPI_Recv( 
      bufferStart(),
      bufferSize(),
      MPI_BYTE, 
      mCurrentSource,
      MpiMessageTag::kWriteData, 
      mCommunicator, 
      0 );
  }

  // call the base class sync to prepare for reading, writing.
  return xdm::BinaryStreamBuffer::sync();
}

int CoalescingStreamBuffer::overflow( int c ) {
  std::streamsize bufferContentSize = pptr() - pbase();

  // If there is content in the buffer, synchronize and return EOF on fail.
  if ( bufferContentSize && sync() ) {
    // sync returns nonzero on fail
    return eof();
  }

  // Buffer has been successfully synchronized. Begin writing again as usual at
  // the beginning of the buffer.
  if ( c != eof() ) {
    return sputc( c );
  }

  return eof();
}

int CoalescingStreamBuffer::uflow() {
  std::streamsize bufferSize = egptr() - eback();

  if ( eback() && bufferSize ) {
    // spin until we have another message from the current source.
    while( !poll( mCurrentSource ) );
    // there is a message from the current source, receive it and continue to
    // read data.
    if ( sync() == 0 ) {
      return sbumpc();
    }
  }

  return eof();
}

} // namespace xdmComm

