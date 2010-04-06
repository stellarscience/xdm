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

BOOST_AUTO_TEST_CASE( parseFile ) {
  xdm::FileSystemPath testFilePath( "test_document1.xmf" );

  xdmf::XmfReader reader;
  xdm::RefPtr< xdm::Item > result = reader.readItem( testFilePath );
  BOOST_CHECK( result );
}

BOOST_AUTO_TEST_CASE( invalidDocument ) {
  const char * kXml = "<Xdmf Version='2.1'><foo/></Xdmf>";
  const char * kTestFileName = "invalidDocumentFile.xmf";

  {
    std::ofstream testfile( kTestFileName );
    testfile << kXml;
  }

  xdmf::XmfReader reader;
  BOOST_CHECK_THROW( reader.readItem( xdm::FileSystemPath( kTestFileName ) ),
    xdmf::ValidationError );
}
