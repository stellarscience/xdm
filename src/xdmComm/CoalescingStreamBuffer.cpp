#include <xdmComm/CoalescingStreamBuffer.hpp>

XDM_COMM_NAMESPACE_BEGIN

CoalescingStreamBuffer::CoalescingStreamBuffer(
  size_t bufSize,
  MPI_Comm communicator ) :
  BinaryStreamBuffer( bufSize ),
  mCommunicator( communicator ) {
}

CoalescingStreamBuffer::~CoalescingStreamBuffer() {
}

void CoalescingStreamBuffer::sync() {
  // get the process rank to decide what to do
  int localRank;
  MPI_Comm_rank( mCommunicator, &localRank );

  if ( localRank != 0 ) {
    // non-zero ranks send to rank 0
    MPI_Ssend( pointer(), size(), MPI_BYTE, 0, 0, mCommunicator );
  } else {
    // rank 0 receives.
    MPI_Recv( pointer(), size(), MPI_BYTE, MPI_ANY_SOURCE, 
      0, mCommunicator, 0 );
  }

  // call the base class sync to prepare for reading, writing.
  BinaryStreamBuffer::sync();
}

XDM_COMM_NAMESPACE_END

