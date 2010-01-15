#include <xdmComm/MpiDatasetProxy.hpp>

#include <xdmComm/BinaryIStream.hpp>
#include <xdmComm/BinaryOStream.hpp>
#include <xdmComm/BinaryStreamOperations.hpp>
#include <xdmComm/CoalescingStreamBuffer.hpp>
#include <xdmComm/MpiMessageTag.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelection.hpp>
#include <xdm/DataSelectionMap.hpp>
#include <xdm/DataSelectionVisitor.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/StructuredArray.hpp>
#include <xdm/VectorStructuredArray.hpp>

XDM_COMM_NAMESPACE_BEGIN

namespace {

// receive and write off core data to a dataset.
// Precondition: There must be a message available to receive.
void receiveAndWriteProcessData( 
  BinaryStreamBuffer* commBuf, 
  xdm::Dataset* dataset,
  xdm::StructuredArray* arrayBuffer ) { 

  BinaryIStream dataStream( commBuf );

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
  xdm::Dataset* dataset,
  size_t bufSizeHint ) :
  mCommunicator( communicator ),
  mDataset( dataset ),
  mCommBuffer( new CoalescingStreamBuffer( bufSizeHint, communicator ) ),
  mArrayBuffer( new xdm::VectorStructuredArray<char>( bufSizeHint ) ) {
}

MpiDatasetProxy::~MpiDatasetProxy() {
}

void MpiDatasetProxy::update() {
  mDataset->update();
  Dataset::update();
}

const char* MpiDatasetProxy::format() {
  return mDataset->format();
}

void MpiDatasetProxy::writeTextContent( xdm::XmlTextContent& text ) {
  mDataset->writeTextContent( text );
}

void MpiDatasetProxy::initializeImplementation(
  xdm::primitiveType::Value type,
  const xdm::DataShape<>& shape ) {
  
  MPI_Barrier( mCommunicator );

  int rank;
  MPI_Comm_rank( mCommunicator, &rank );
  if ( rank == 0 ) {
    mDataset->initialize( type, shape );
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
    BinaryOStream dataStream( mCommBuffer.get() );
    dataStream << *array;
    dataStream << selectionMap;
    dataStream.flush();
  
  } else {
    
    // write local process data to the dataset.
    mDataset->serialize( array, selectionMap );

    // Check for any messages that may have come from other processes.
    while ( mCommBuffer->poll() ) {
      receiveAndWriteProcessData( 
        mCommBuffer.get(), 
        mDataset.get(),
        mArrayBuffer.get() );
    }
  }
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
          mDataset.get(),
          mArrayBuffer.get() );
      }
    }
    mDataset->finalize();
  } else {
    // Not rank 0 and local process is done with current dataset.  Signal.
    MPI_Ssend( datasetCompleteSignalBuffer, 1, MPI_BYTE, 0, 
      MpiMessageTag::kProcessCompleted, mCommunicator );
  }
}

XDM_COMM_NAMESPACE_END

