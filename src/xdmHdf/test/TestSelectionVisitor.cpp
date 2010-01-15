#define BOOST_TEST_MODULE 
#include <boost/test/unit_test.hpp>

#include <xdmHdf/SelectionVisitor.hpp>

#include <xdm/CoordinateDataSelection.hpp>
#include <xdm/DataSelection.hpp>
#include <xdm/HyperslabDataSelection.hpp>

#include <hdf5.h>

#include <vector>

struct SelectionVisitor : ::testing::Test {
  hid_t dataspace;
  SelectionVisitor() {
    hsize_t rank = 2;
    hsize_t dims[] = {2, 2};
    dataspace = H5Screate_simple( rank, dims, NULL );
  }
  ~SelectionVisitor() {
    H5Sclose( dataspace );
  }
};

/*

Test removed due to 32/64 bit compatibility.  For more information, look at the
CoordinateDataSelection class declaration in CoordinateDataSelection.hpp

BOOST_AUTO_TEST_CASE( applyCoordinateSelection ) {
  std::vector< size_t > coords;
  coords.push_back( 1 );
  coords.push_back( 1 );
  xdm::CoordinateDataSelection selection( 
    xdm::CoordinateArray<>( &coords[0], 2, 1 ) );
  xdmHdf::SelectionVisitor visitor( dataspace );
  selection.accept( visitor );

  hsize_t result[1][2];
  H5Sget_select_elem_pointlist( dataspace, 0, 1, 
    reinterpret_cast< hsize_t* >( result ) );

  hsize_t answer[1][2];
  answer[0][0] = 1;
  answer[0][1] = 1;

  ASSERT_EQ( H5S_SEL_POINTS, H5Sget_select_type( dataspace ) );
  ASSERT_EQ( answer[0][0], result[0][0] );
  ASSERT_EQ( answer[0][1], result[0][1] );
}
*/

BOOST_AUTO_TEST_CASE( applyHyperslabSelection ) {
  xdm::DataShape<> shape( 2 );
  shape[0] = 2;
  shape[1] = 2;
  xdm::HyperSlab<> slab( shape );
  slab.setStart( 0, 1 );
  slab.setStart( 1, 1 );
  slab.setStride( 0, 1 );
  slab.setStride( 1, 1 );
  slab.setCount( 0, 1 );
  slab.setCount( 1, 1 );

  xdm::HyperslabDataSelection selection( slab );
  xdmHdf::SelectionVisitor visitor( dataspace );
  selection.accept( visitor );

  ASSERT_EQ( H5S_SEL_HYPERSLABS, H5Sget_select_type( dataspace ) );

  hssize_t numblocks = H5Sget_select_hyper_nblocks( dataspace );
  ASSERT_EQ( 1, numblocks );

  hsize_t result[2][2];
  H5Sget_select_hyper_blocklist( dataspace, 0, 1, 
    reinterpret_cast< hsize_t* >( result ) );

  hsize_t answer[2][2];
  answer[0][0] = 1;
  answer[0][1] = 1;
  answer[1][0] = 1;
  answer[1][1] = 1;
  ASSERT_EQ( answer[0][0], result[0][0] );
  ASSERT_EQ( answer[0][1], result[0][1] );
  ASSERT_EQ( answer[1][0], result[1][0] );
  ASSERT_EQ( answer[1][1], result[1][1] );
}

int main( int argc, char* argv[] ) {
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}

