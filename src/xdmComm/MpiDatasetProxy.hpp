#ifndef xdmComm_MpiDatasetProxy_hpp
#define xdmComm_MpiDatasetProxy_hpp

#include <xdm/Dataset.hpp>

#include <mpi.h>

#include <memory>
#include <vector>

#include <xdmComm/NamespaceMacro.hpp>

namespace xdmComm {
  class ReceiveBufferArray;
}

namespace xdm {
  class StructuredArray;
}

XDM_COMM_NAMESPACE_BEGIN

class CoalescingStreamBuffer;

/// Dataset proxy that uses MPI to communicate data between processes before
/// actually writing the data to a dataset.  The communication procedures assume
/// that the cluster is heterogeneous and the objects to be sent are bitwise
/// serializable.
class MpiDatasetProxy : public xdm::Dataset {
public:
  /// Construct a proxy for the input dataset.
  /// @param communicator Communicator with relevant processes.
  /// @param dataset The actual dataset that will handle writing.
  /// @param bufSizeHint Suggested size for communication buffer.
  MpiDatasetProxy( 
    MPI_Comm communicator, 
    xdm::Dataset* dataset,
    size_t bufSizeHint );

  virtual ~MpiDatasetProxy();

  /// Update call to update myself and the real dataset.
  virtual void update();

  /// Pass-through to get the format of the underlying dataset.
  virtual const char* format();
  /// Pass-through to get the real dataset to write it's text content.
  virtual void writeTextContent( xdm::XmlTextContent& text );

protected:
  /// Initialization calls underlying dataset initialization only if this
  /// process is rank 0 within the communicator.
  virtual void initializeImplementation( 
    xdm::primitiveType::Value type,
    const xdm::DataShape<>& shape );

  /// Serialization process packs and sends the array and selection data to rank
  /// 0 in the communicator for writing.
  virtual void serializeImplementation( 
    const xdm::StructuredArray* data,
    const xdm::DataSelectionMap& selectionMap );

  /// Finalization calls underlying dataset finalization only if this process is
  /// rank 0 in the assigned communicator.
  virtual void finalizeImplementation();

private:
  MPI_Comm mCommunicator;
  xdm::RefPtr< xdm::Dataset > mDataset;
  std::auto_ptr< xdmComm::CoalescingStreamBuffer > mCommBuffer;
  xdm::RefPtr< xdmComm::ReceiveBufferArray > mArrayBuffer;
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_MpiDatasetProxy_hpp

