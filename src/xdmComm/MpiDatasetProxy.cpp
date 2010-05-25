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
#include <xdmComm/MpiDatasetProxy.hpp>

#include <xdmComm/BarrierOnExit.hpp>
#include <xdmComm/CoalescingStreamBuffer.hpp>
#include <xdmComm/MpiMessageTag.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/BinaryIStream.hpp>
#include <xdm/BinaryOStream.hpp>
#include <xdm/BinaryStreamOperations.hpp>
#include <xdm/ByteArray.hpp>
#include <xdm/DataSelection.hpp>
#include <xdm/DataSelectionMap.hpp>
#include <xdm/DataSelectionVisitor.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/StructuredArray.hpp>
#include <xdm/VectorStructuredArray.hpp>

namespace xdmComm {

namespace {

// receive and write off core data to a dataset.
// Precondition: There must be a message available to receive.
void receiveAndWriteProcessData( 
  xdm::BinaryStreamBuffer* commBuf,
  xdm::Dataset* dataset,
  xdm::ByteArray* arrayBuffer ) {

  xdm::BinaryIStream dataStream( commBuf );

  // synchronize the stream to receive from a single process.
  dataStream.sync();

  // reconstruct the information from the message
  xdm::DataSelectionMap processSelectionMap;
  dataStream >> *arrayBuffer;
  dataStream >> processSelectionMap;

  // write the process data to the dataset.
  dataset->serialize( arrayBuffer, processSelectionMap );
}

} // namespace anon

MpiDatasetProxy::MpiDatasetProxy( 
  MPI_Comm communicator, 
  xdm::RefPtr< xdm::Dataset > dataset,
  size_t bufSizeHint ) :
  xdm::ProxyDataset( dataset ),
  mCommunicator( communicator ),
  mCommBuffer( new CoalescingStreamBuffer( bufSizeHint, communicator ) ),
  mArrayBuffer( new xdm::ByteArray( bufSizeHint ) ) {
}

MpiDatasetProxy::~MpiDatasetProxy() {
}

xdm::DataShape<> MpiDatasetProxy::initializeImplementation(
  xdm::primitiveType::Value type,
  const xdm::DataShape<>& shape,
  const xdm::Dataset::InitializeMode& mode ) {
  
  MPI_Barrier( mCommunicator );

  int rank;
  MPI_Comm_rank( mCommunicator, &rank );
  if ( rank == 0 ) {
    return xdm::ProxyDataset::initializeImplementation( type, shape, mode );
  } else {
    return xdm::DataShape<>(); // size of dataset is 0.
  }
}

void MpiDatasetProxy::serializeImplementation(
  const xdm::StructuredArray* array,
  const xdm::DataSelectionMap& selectionMap ) {

  int localRank;
  MPI_Comm_rank( mCommunicator, &localRank );

  // Rank 0 in the communicator writes local data and polls for messages from
  // other processes.
  if ( localRank != 0 ) {
    xdm::BinaryOStream dataStream( mCommBuffer.get() );
    dataStream << *array;
    dataStream << selectionMap;
    dataStream << xdm::flush;
  
  } else {
    
    // write local process data to the dataset.
    xdm::ProxyDataset::serializeImplementation( array, selectionMap );

    // Check for any messages that may have come from other processes.
    while ( mCommBuffer->poll() ) {
      receiveAndWriteProcessData( 
        mCommBuffer.get(), 
        innerDataset().get(),
        mArrayBuffer.get() );
    }
  }
}

void MpiDatasetProxy::deserializeImplementation(
  xdm::StructuredArray *data,
  const xdm::DataSelectionMap &selectionMap ) {

  xdm::ProxyDataset::deserializeImplementation( data, selectionMap );

}

void MpiDatasetProxy::finalizeImplementation() {
  int size;
  MPI_Comm_size( mCommunicator, &size );
  int rank;
  MPI_Comm_rank( mCommunicator, &rank );

  char datasetCompleteSignalBuffer[1];
  datasetCompleteSignalBuffer[0] = 1;

  if ( rank == 0 ) {
    
    // wait for all processes to signal that they are done with this dataset
    int processesCompleted = 1;
    while ( processesCompleted < size ) {
      MPI_Status status;
      int flag;
      MPI_Iprobe(
        MPI_ANY_SOURCE, 
        MpiMessageTag::kProcessCompleted,
        mCommunicator,
        &flag,
        &status );
      
      // if a process signalled complete, swallow the message and increment
      if ( flag ) {
        MPI_Recv( datasetCompleteSignalBuffer, 1, MPI_BYTE, MPI_ANY_SOURCE,
          MpiMessageTag::kProcessCompleted, mCommunicator, &status );
        processesCompleted++;
      }

      // poll and process any new messages
      while ( mCommBuffer->poll() ) {
        receiveAndWriteProcessData(
          mCommBuffer.get(),
          innerDataset().get(),
          mArrayBuffer.get() );
      }
    }
    xdm::ProxyDataset::finalizeImplementation();
  } else {
    // Not rank 0 and local process is done with current dataset.  Signal.
    MPI_Ssend( datasetCompleteSignalBuffer, 1, MPI_BYTE, 0, 
      MpiMessageTag::kProcessCompleted, mCommunicator );
  }
}

} // namespace xdmComm

