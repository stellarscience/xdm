//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmComm/CoalescingStreamBuffer.hpp>
#include <xdmComm/MpiMessageTag.hpp>

XDM_COMM_NAMESPACE_BEGIN

CoalescingStreamBuffer::CoalescingStreamBuffer(
  size_t bufSize,
  MPI_Comm communicator ) :
  BinaryStreamBuffer( bufSize ),
  mCommunicator( communicator ) {
}

CoalescingStreamBuffer::~CoalescingStreamBuffer() {
}

bool CoalescingStreamBuffer::poll() const {
  MPI_Status status;
  int flag;

  MPI_Iprobe( 
    MPI_ANY_SOURCE, 
    MpiMessageTag::kWriteData, 
    mCommunicator,
    &flag,
    &status );

  return flag;
}

void CoalescingStreamBuffer::sync() {
  // get the process rank to decide what to do
  int localRank;
  MPI_Comm_rank( mCommunicator, &localRank );

  if ( localRank != 0 ) {
    // non-zero ranks send to rank 0
    MPI_Ssend( 
      pointer(), 
      size(), 
      MPI_BYTE, 
      0, 
      MpiMessageTag::kWriteData, 
      mCommunicator );
  } else {
    // rank 0 receives.
    MPI_Recv( 
      pointer(), 
      size(), 
      MPI_BYTE, 
      MPI_ANY_SOURCE, 
      MpiMessageTag::kWriteData, 
      mCommunicator, 
      0 );
  }

  // call the base class sync to prepare for reading, writing.
  BinaryStreamBuffer::sync();
}

XDM_COMM_NAMESPACE_END

