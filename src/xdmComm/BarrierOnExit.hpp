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
#ifndef xdmComm_BarrierOnExit_hpp
#define xdmComm_BarrierOnExit_hpp

#include <mpi.h>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Provides an exception safe MPI_Barrier upon exit of the current scope.
/// This class provides safety from deadlock when a barrier is required at
/// scope exit and any process within the given communicator throws an
/// exception. This RAII class should always be preferred over an explicit
/// MPI_Barrier call.
class BarrierOnExit {
public:
  BarrierOnExit( MPI_Comm communicator );
  ~BarrierOnExit();

private:
  MPI_Comm mCommunicator;
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_BarrierOnExit_hpp
