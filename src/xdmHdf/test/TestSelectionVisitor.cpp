//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.      
//                                                                             
// This file is part of XDM                                                    
//                                                                             
// This program is free software: you can redistribute it and/or modify it     
// under the terms of the GNU Lesser General Public License as published by    
// the Free Software Foundation, either version 3 of the License, or (at your  
// option) any later version.                                                  
//                                                                             
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        
// License for more details.                                                   
//                                                                             
// You should have received a copy of the GNU Lesser General Public License    
// along with this program.  If not, see <http://www.gnu.org/licenses/>.       
//                                                                             
//------------------------------------------------------------------------------
#define BOOST_TEST_MODULE TestSelectionVisitor 
#include <boost/test/unit_test.hpp>

#include <xdmHdf/SelectionVisitor.hpp>

#include <xdm/CoordinateDataSelection.hpp>
#include <xdm/DataSelection.hpp>
#include <xdm/HyperslabDataSelection.hpp>

#include <hdf5.h>

#include <vector>

namespace {

struct Fixture {
  hid_t dataspace;
  Fixture() {
    hsize_t rank = 2;
    hsize_t dims[] = {2, 2};
    dataspace = H5Screate_simple( rank, dims, NULL );
  }
  ~Fixture() {
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

  BOOST_CHECK_EQUAL( H5S_SEL_POINTS, H5Sget_select_type( dataspace ) );
  BOOST_CHECK_EQUAL( answer[0][0], result[0][0] );
  BOOST_CHECK_EQUAL( answer[0][1], result[0][1] );
}
*/

BOOST_AUTO_TEST_CASE( applyHyperslabSelection ) {
  Fixture test;

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
  xdmHdf::SelectionVisitor visitor( test.dataspace );
  selection.accept( visitor );

  BOOST_CHECK_EQUAL( H5S_SEL_HYPERSLABS, H5Sget_select_type( test.dataspace ) );

  hssize_t numblocks = H5Sget_select_hyper_nblocks( test.dataspace );
  BOOST_CHECK_EQUAL( 1, numblocks );

  hsize_t result[2][2];
  H5Sget_select_hyper_blocklist( test.dataspace, 0, 1, 
    reinterpret_cast< hsize_t* >( result ) );

  hsize_t answer[2][2];
  answer[0][0] = 1;
  answer[0][1] = 1;
  answer[1][0] = 1;
  answer[1][1] = 1;
  BOOST_CHECK_EQUAL( answer[0][0], result[0][0] );
  BOOST_CHECK_EQUAL( answer[0][1], result[0][1] );
  BOOST_CHECK_EQUAL( answer[1][0], result[1][0] );
  BOOST_CHECK_EQUAL( answer[1][1], result[1][1] );
}

} // namespace 

