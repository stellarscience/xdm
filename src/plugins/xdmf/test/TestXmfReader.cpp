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
#define BOOST_TEST_MODULE XmfReader 
#include <boost/test/unit_test.hpp>

#include <xdmf/XmfReader.hpp>

#include <xdm/Item.hpp>

#include <fstream>

char const * const xml = 
  "<Xdmf Version='2.1'>"
  "</Xdmf>";

BOOST_AUTO_TEST_CASE( parseFile ) {
  xdm::FileSystemPath testFilePath( "xmfParseFileTest.xmf" );
  if ( xdm::exists( testFilePath ) ) {
    xdm::remove( testFilePath );
  }
  
  {
    // write the file for the test
    std::ofstream out( testFilePath.pathString().c_str() );
    out << xml;
  }

  xdmf::XmfReader reader;
  xdm::RefPtr< xdm::Item > result = reader.readItem( testFilePath );
  BOOST_CHECK( result );
}
