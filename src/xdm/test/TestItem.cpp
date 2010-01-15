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
#define BOOST_TEST_MODULE Item 
#include <boost/test/unit_test.hpp>

#include <xdm/Item.hpp>

namespace {

BOOST_AUTO_TEST_CASE( writeMetadataTag ) {
  xdm::RefPtr< xdm::Item > i( new xdm::Item );
  i->setName( "Fred" );

  xdm::XmlMetadataWrapper xml( new xdm::XmlObject );
  i->writeMetadata( xml );

  char const * const tagAnswer = "Item";
  std::string tagResult = xml.tag();
  BOOST_CHECK_EQUAL( tagAnswer, tagResult );

  char const * const nameAnswer = "Fred";
  std::string nameResult = xml.attribute( "Name" );
  BOOST_CHECK_EQUAL( nameAnswer, nameResult );
}

} // namespace

