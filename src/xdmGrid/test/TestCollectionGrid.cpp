//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.      
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
#define BOOST_TEST_MODULE CollectionGrid 
#include <boost/test/unit_test.hpp>

#include <xdmGrid/CollectionGrid.hpp>

namespace {

struct Fixture {
  xdm::RefPtr< xdmGrid::CollectionGrid > grid;
  xdm::XmlMetadataWrapper xml;
  Fixture() : 
    grid( new xdmGrid::CollectionGrid ),
    xml( new xdm::XmlObject ) {}
};

BOOST_AUTO_TEST_CASE( writeMetadataSpatial ) {
  Fixture test;

  test.grid->setType( xdmGrid::CollectionGrid::kSpatial );
  test.grid->writeMetadata( test.xml );

  BOOST_CHECK_EQUAL( "Grid", test.xml.tag() );
  BOOST_CHECK_EQUAL( "Collection", test.xml.attribute( "GridType" ) );
  BOOST_CHECK_EQUAL( "Spatial", test.xml.attribute( "CollectionType" ) );
}

BOOST_AUTO_TEST_CASE( writeMetadataTemporal ) {
  Fixture test;

  test.grid->setType( xdmGrid::CollectionGrid::kTemporal );
  test.grid->writeMetadata( test.xml );

  BOOST_CHECK_EQUAL( "Grid", test.xml.tag() );
  BOOST_CHECK_EQUAL( "Collection", test.xml.attribute( "GridType" ) );
  BOOST_CHECK_EQUAL( "Temporal", test.xml.attribute( "CollectionType" ) );
}

} // namespace 

