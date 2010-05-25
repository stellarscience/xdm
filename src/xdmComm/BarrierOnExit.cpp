#include <xdmComm/BarrierOnExit.hpp>

namespace xdmComm {

BarrierOnExit::BarrierOnExit( MPI_Comm communicator ) :
  mCommunicator( communicator ) {
}

BarrierOnExit::~BarrierOnExit() {
  MPI_Barrier( mCommunicator );
}

} // namespace xdmComm
