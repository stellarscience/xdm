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
#define BOOST_TEST_MODULE XmlObject 
#include <boost/test/unit_test.hpp>

#include <xdm/XmlObject.hpp>

#include <sstream>

namespace {

using xdm::XmlObject;

xdm::RefPtr< XmlObject > buildTree() {
  xdm::RefPtr< XmlObject > foo( new XmlObject( "foo" ) );
  foo->appendAttribute( "bar", "42" );
  xdm::RefPtr< XmlObject > baz( new XmlObject( "baz" ) );
  foo->appendChild( baz );
  baz->appendContent( "quack" );
  baz->appendChild( new XmlObject( "quux" ) );
  return foo;
}

BOOST_AUTO_TEST_CASE( printHeader ) {
  xdm::RefPtr< XmlObject > obj( new XmlObject( "obj" ) );

  char const * const answer = "<obj>\n";

  std::stringstream result;
  obj->printHeader( result );

  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( printTextContent ) {
  xdm::RefPtr< XmlObject > obj( new XmlObject( "obj" ) );
  obj->appendContent( "text content" );
  BOOST_CHECK_EQUAL( "text content", obj->contentLine(0) );

  char const * const answer = "text content\n";

  std::stringstream result;
  obj->printTextContent( result );
  obj->printChildren( result );

  BOOST_CHECK_EQUAL( answer, result.str() );
}

BOOST_AUTO_TEST_CASE( OStreamInsertion ) {
  char const * const answer = 
    "<foo bar='42'>\n"
    "  <baz>\n"
    "    quack\n"
    "    <quux>\n"
    "    </quux>\n"
    "  </baz>\n"
    "</foo>\n";
  xdm::RefPtr< XmlObject > obj = buildTree();
  std::stringstream result;
  result << *obj;
  BOOST_CHECK_EQUAL( answer, result.str() );
}

} // namespace

