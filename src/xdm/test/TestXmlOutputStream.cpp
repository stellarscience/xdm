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
#define BOOST_TEST_MODULE XmlOutputStream 
#include <boost/test/unit_test.hpp>

#include <xdm/XmlObject.hpp>
#include <xdm/XmlOutputStream.hpp>

#include <sstream>

namespace {

using xdm::RefPtr;
using xdm::XmlObject;
using xdm::XmlOutputStream;

BOOST_AUTO_TEST_CASE( openContext ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );
  
  std::stringstream result;
  XmlOutputStream test( result );
  test.openContext( obj );
  
  char const * const answer = "<?xml version='1.0'?>\n<obj>\n";
  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( writeSingleObject ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  test.writeObject( obj );

  char const * const answer =
    "<?xml version='1.0'?>\n"
    "<obj>\n"
    "</obj>\n";
  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( writeStreamingObject ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );
  RefPtr< XmlObject > chi( new XmlObject( "chi" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  test.openContext( obj );
  test.writeObject( chi );

  char const * const answer_incomplete = 
    "<?xml version='1.0'?>\n"
    "<obj>\n"
    "  <chi>\n"
    "  </chi>\n";
  BOOST_CHECK_EQUAL( answer_incomplete, result.str() );

  test.closeCurrentContext();

  char const * const answer_complete =
    "<?xml version='1.0'?>\n"
    "<obj>\n"
    "  <chi>\n"
    "  </chi>\n"
    "</obj>\n";
  BOOST_CHECK_EQUAL( answer_complete, result.str() );
}

BOOST_AUTO_TEST_CASE( closeCurrentContext ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  test.openContext( obj );
  test.closeCurrentContext();

  char const * const answer = 
    "<?xml version='1.0'?>\n"
    "<obj>\n"
    "</obj>\n";
  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( closeStream ) {
  RefPtr< XmlObject > obj( new XmlObject( "obj" ) );
  RefPtr< XmlObject > chi( new XmlObject( "chi" ) );

  std::stringstream result;
  XmlOutputStream test( result );
  
  test.openContext( obj );
  test.openContext( chi );
  test.closeStream();
}

} // namespace

