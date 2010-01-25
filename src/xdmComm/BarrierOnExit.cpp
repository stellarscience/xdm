#include <xdmComm/BarrierOnExit.hpp>

XDM_COMM_NAMESPACE_BEGIN

BarrierOnExit::BarrierOnExit( MPI_Comm communicator ) :
  mCommunicator( communicator ) {
}

BarrierOnExit::~BarrierOnExit() {
  MPI_Barrier( mCommunicator );
}

XDM_COMM_NAMESPACE_END
