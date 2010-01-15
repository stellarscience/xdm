#include <xdm/StructuredArray.hpp>
#include <xdm/RefPtr.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <sstream>

int main( int argc, char* argv[] ) {
  
  // set up the input data
  std::vector< float > data(16);
  for( int i = 0; i < 16; ++i ) {
    data[i] = i;
  }
  
  xdm::DataShape<> arrayShape( 1 );
  arrayShape[0] = 16;
  xdm::StructuredArray array( xdm::primitiveType::kFloat, sizeof(float), 
    &data[0], arrayShape );

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
  xdm::RefPtr< xdmHdf::HdfDataset > dataset = new xdmHdf::HdfDataset();
  dataset->setFile( "HdfDataset.h5" );
  dataset->setGroup( "" );
  dataset->setDataset( "testdata" );

  // write the data to disk
  dataset->initialize( xdm::primitiveType::kFloat, fileshape );
  dataset->serialize( array, mapping );
  dataset->finalize();
  
  return 0;
}

