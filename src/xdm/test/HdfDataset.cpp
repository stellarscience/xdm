#include <xdm/Array.hpp>
#include <xdm/HdfDataset.hpp>

#include <sstream>

int main( int argc, char* argv[] ) {
  
  // set up the input data
  xdm::DataShape<> arrayShape( 1 );
  arrayShape[0] = 16;
  xdm::Array data( arrayShape );
  float* ptr = data.data();
  for( int i = 0; i < 16; ++i ) {
    ptr[i] = i;
  }

  // initialize the dataset on disk
  xdm::HdfDataset* dataset = new xdm::HdfDataset();
  xdm::DataShape<> fileshape( 2 );
  fileshape[0] = 4;
  fileshape[1] = 4;
  std::stringstream init( "HdfDataset.h5:/testdata" );
  dataset->initialize( fileshape, init );

  // set up the mapping from memory space to file space
  xdm::Slab<> memorySlab( arrayShape );
  memorySlab.setStart( 0, 0 );
  memorySlab.setStride( 0, 1 );
  memorySlab.setCount( 0, 16 );
  xdm::Slab<> fileSlab( fileshape );
  fileSlab.setStart( 0, 0 );
  fileSlab.setStart( 1, 0 );
  fileSlab.setStride( 0, 1 );
  fileSlab.setStride( 1, 1 );
  fileSlab.setCount( 0, 4 );
  fileSlab.setCount( 1, 4 );
  xdm::SlabMap<> mapping( memorySlab, fileSlab );

  // write the data to disk
  std::stringstream content;
  dataset->serialize( data, mapping, content );

  // finalize the process
  dataset->finalize();
  return 0;
}

