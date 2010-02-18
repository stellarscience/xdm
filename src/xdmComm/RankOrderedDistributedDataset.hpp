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
#ifndef xdmComm_RankOrderedDistributedDataset_hpp
#define xdmComm_RankOrderedDistributedDataset_hpp

#include <xdm/ProxyDataset.hpp>

#include <mpi.h>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Dataset proxy that arranges dataset write requests from multiple processes
/// in order of rank within a given communicator. That is, the size of the
/// first dimension of the underlying dataset will be expanded to hold the data
/// from each participating process and the data from each participating process
/// will be arrayed contiguously in the first dimension.
///
/// For example, if all processes write n x 3 data elements, and the
/// communicator has 5 processes, then the resultant dataset will be 5*n x 3
/// elements in size with rank 0's data occupying the first n elements, rank 1's
/// data occupying the next n elements and so on.
class RankOrderedDistributedDataset : public xdm::ProxyDataset {
public:
  RankOrderedDistributedDataset(
    xdm::RefPtr< xdm::Dataset > dataset,
    MPI_Comm communicator );
  virtual ~RankOrderedDistributedDataset();

protected:

  /// Initialize determines the shape that all participating processes are
  /// requesting and expands the first dimension of the inner dataset to hold
  /// each one of them.
  ///
  /// All processes must be requesting the same type of access, and the
  /// data shape for all processes must match in every dimension except the
  /// first.
  virtual xdm::DataShape<> initializeImplementation(
    xdm::primitiveType::Value type,
    const xdm::DataShape<>& shape,
    const xdm::Dataset::InitializeMode& mode );

  /// Serialize uses the information that was distributed to all processes
  /// during initialization to convert the input selection to a hyperslab or
  /// coordinate selection that will offset the first dimension so that all
  /// processes write their data in rank order.
  virtual void serializeImplementation(
    const xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );

private:
  MPI_Comm mCommunicator;
  xdm::DataShape<>::size_type mStartLocation;
  xdm::DataShape<> mDataShape;
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_RankOrderedDistributedDataset_hpp
