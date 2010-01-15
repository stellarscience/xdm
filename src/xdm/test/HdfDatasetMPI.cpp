#include <xdm/Array.hpp>
#include <xdm/HdfDataset.hpp>

#include <mpi.h>

#include <iostream>
#include <sstream>

class CommunicationCallback : 
  public xdm::InitializeCallback,
  public xdm::SerializeCallback,
  public xdm::FinalizeCallback {
public:
  int m_rank;
  int m_processes;

  CommunicationCallback() {
    MPI_Comm_size( MPI_COMM_WORLD, &m_processes );
    MPI_Comm_rank( MPI_COMM_WORLD, &m_rank );
  }

  void initializeImplementation(
    xdm::Dataset* ds,
    const xdm::DataShape<>& shape,
    std::iostream& content ) {
    if ( m_rank == 0 ) {
      ds->initializeImplementation( shape, content );
    }
  }

  void serializeImplementation( 
    xdm::Dataset* ds,
    const xdm::Array& data,
    const xdm::SlabMap<>& memory_map,
    std::iostream& content ) {
    
    // allocate a buffer of the correct to send or receive information
    // this assumes that all processes require the same amount of space
    // the memory map and the actual data need to be sent
    size_t bufsz = sizeof( xdm::SlabMap<> ) + data.size();
    char* buf = new char[bufsz];

    if ( m_rank != 0 ) {
      memcpy( buf, &memory_map, sizeof( xdm::SlabMap<> ) );
      memcpy( buf + sizeof( xdm::SlabMap<> ), data.data(), data.size() );
      MPI_Send( buf, bufsz, MPI_BYTE, 0, 0, MPI_COMM_WORLD );
    } else {
      // rank 0 receives and writes all data
      // first write my local data
      ds->serializeImplementation( data, memory_map, content );

      // now receive data from everyone else
      int processes_received = 1; // so far, I've only handled my own data
      while ( processes_received < m_processes ) {
        // receive some off core data
        MPI_Recv( buf, bufsz, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, 0 );
        processes_received++;

        // pull the data from the buffer
        xdm::SlabMap<> rcv_mem_map;
        memcpy( &rcv_mem_map, buf, sizeof( xdm::SlabMap<> ) );
        xdm::Array rcv_data( rcv_mem_map.domain().shape() );
        memcpy( rcv_data.data(), buf + sizeof( xdm::SlabMap<> ), data.size() );

        // now write the received data
        ds->serializeImplementation( rcv_data, rcv_mem_map, content );
      } // while ( processes_received < m_processes
    } // else (matches if m_rank != 0 )
    
    delete [] buf;
  }

  void finalizeImplementation( xdm::Dataset* ds ) {
    if ( m_rank == 0 ) {
      ds->finalizeImplementation();
    }
  }
};

int main( int argc, char* argv[] ) {
  MPI_Init( &argc, &argv );

  int rank;
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  // set up the input data
  xdm::DataShape<> arrayShape( 1 );
  arrayShape[0] = 4;
  xdm::Array data( arrayShape );
  float* ptr = data.data();
  for( int i = 0; i < 4; ++i ) {
    ptr[i] = rank;
  }

  // initialize the dataset on disk
  xdm::DataShape<> fileshape( 2 );
  fileshape[0] = 4;
  fileshape[1] = 4;

  // set up the mapping from memory space to file space
  xdm::Slab<> memorySlab( arrayShape );
  memorySlab.setStart( 0, 0 );
  memorySlab.setStride( 0, 1 );
  memorySlab.setCount( 0, 4 );
  xdm::Slab<> fileSlab( arrayShape );
  fileSlab.setStart( 0, rank );
  fileSlab.setStart( 1, 0 );
  fileSlab.setStride( 0, 1 );
  fileSlab.setStride( 1, 1 );
  fileSlab.setCount( 0, 2 );
  fileSlab.setCount( 1, 2 );
  xdm::SlabMap<> mapping( memorySlab, fileSlab );

  // create the dataset
  xdm::HdfDataset* dataset = new xdm::HdfDataset();

  // install the callbacks
  CommunicationCallback* cb = new CommunicationCallback();
  dataset->setInitializeCallback( cb );
  dataset->setSerializeCallback( cb );
  dataset->setFinalizeCallback( cb );

  // write the data to disk
  std::stringstream content( "HdfDatasetMPI.h5:/testdata" );
  dataset->initialize( fileshape, content );
  dataset->serialize( data, mapping, content );
  dataset->finalize();
  
  MPI_Finalize();
  return 0;
}

