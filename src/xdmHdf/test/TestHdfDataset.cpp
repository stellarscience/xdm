#define BOOST_TEST_MODULE TestHdfDataset
#include <boost/test/unit_test.hpp>

#include <xdm/TemplateStructuredArray.hpp>
#include <xdm/RefPtr.hpp>

#include <xdm/DataSelection.hpp>

#include <xdmHdf/HdfDataset.hpp>

#include <sstream>

namespace {

BOOST_AUTO_TEST_CASE( write ) {
  
  // set up the input data
  std::vector< float > data(16);
  for( int i = 0; i < 16; ++i ) {
    data[i] = i;
  }
  
  xdm::RefPtr< xdm::StructuredArray > array = xdm::createStructuredArray(
    &data[0], 16 );

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
  dataset->serialize( array, xdm::DataSelectionMap() );
  dataset->finalize();
  
  BOOST_CHECK( true );
}

} // namespace

