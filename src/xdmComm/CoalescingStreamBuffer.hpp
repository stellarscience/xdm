#ifndef xdmComm_CoalescingStreamBuffer_hpp
#define xdmComm_CoalescingStreamBuffer_hpp

#include <xdmComm/BinaryStreamBuffer.hpp>

#include <mpi.h>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Stream buffer implementation that synchronizes data between processes in a
/// given communicator.  Processes with rank > 0 in the input communicator will
/// communicate with an MPI synchronous send while rank 0 in the communicator
/// will receive the data.
class CoalescingStreamBuffer : public BinaryStreamBuffer {
private:
  MPI_Comm mCommunicator;

public:
  /// Constructor initializes the communicator and the buffer size.
  CoalescingStreamBuffer( size_t bufSize, MPI_Comm communicator );

  virtual ~CoalescingStreamBuffer();

protected:
  /// Implementation of virtual sync from BinaryStreamBuffer.  Passes data
  /// between processes with send/receive depending upon process rank within an
  /// MPI communicator.
  virtual void sync();
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_CoalescingStreamBuffer_hpp

