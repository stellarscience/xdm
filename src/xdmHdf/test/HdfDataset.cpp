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

  // create the dataset
  xdm::RefPtr< xdmHdf::HdfDataset > dataset = new xdmHdf::HdfDataset();
  dataset->setFile( "HdfDataset.h5" );
  dataset->setDataset( "testdata" );

  // write the data to disk
  dataset->initialize( xdm::primitiveType::kFloat, fileshape );
  dataset->serialize( &array );
  dataset->finalize();
  
  return 0;
}

