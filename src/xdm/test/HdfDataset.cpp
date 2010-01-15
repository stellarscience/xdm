#include <xdm/Array.hpp>
#include <xdm/HdfDataset.hpp>
#include <xdm/RefPtr.hpp>

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
  xdm::DataShape<> fileshape( 2 );
  fileshape[0] = 4;
  fileshape[1] = 4;

  // set up the mapping from memory space to file space
  xdm::HyperSlab<> memorySlab( arrayShape );
  memorySlab.setStart( 0, 0 );
  memorySlab.setStride( 0, 1 );
  memorySlab.setCount( 0, 16 );
  xdm::HyperSlab<> fileSlab( fileshape );
  fileSlab.setStart( 0, 0 );
  fileSlab.setStart( 1, 0 );
  fileSlab.setStride( 0, 1 );
  fileSlab.setStride( 1, 1 );
  fileSlab.setCount( 0, 4 );
  fileSlab.setCount( 1, 4 );
  xdm::HyperSlabMap<> mapping( memorySlab, fileSlab );

  // create the dataset
  xdm::RefPtr< xdm::Dataset > dataset = new xdm::HdfDataset();

  // write the data to disk
  std::stringstream content( "HdfDataset.h5:/testdata" );
  dataset->initialize( fileshape, content );
  dataset->serialize( data, mapping, content );
  dataset->finalize();
  
  return 0;
}

