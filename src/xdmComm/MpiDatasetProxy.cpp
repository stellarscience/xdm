#include <xdmComm/MpiDatasetProxy.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelection.hpp>
#include <xdm/DataSelectionMap.hpp>
#include <xdm/DataSelectionVisitor.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/HyperslabDataSelection.hpp>
#include <xdm/StructuredArray.hpp>

XDM_COMM_NAMESPACE_BEGIN

namespace {

} // namespace anon

MpiDatasetProxy::MpiDatasetProxy( 
  MPI_Comm communicator, 
  xdm::Dataset* dataset,
  size_t bufSizeHint ) :
  mCommunicator( communicator ),
  mDataset( dataset ),
  mCommBuffer( bufSizeHint ) {
}

MpiDatasetProxy::~MpiDatasetProxy() {
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
  int rank;
  MPI_Comm_rank( mCommunicator, &rank );
  if ( rank == 0 ) {
    mDataset->initialize( type, shape );
  }
}

void MpiDatasetProxy::serializeImplementation(
  const xdm::StructuredArray* array,
  const xdm::DataSelectionMap& selectionMap ) {
}

void MpiDatasetProxy::finalizeImplementation() {
  int rank;
  MPI_Comm_rank( mCommunicator, &rank );
  if ( rank == 0 ) {
    mDataset->finalize();
  }
}

XDM_COMM_NAMESPACE_END

