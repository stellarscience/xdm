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
#include <xdm/XmlOutputStream.hpp>

#include <xdm/XmlObject.hpp>

#include <algorithm>

#include <cassert>

namespace xdm {

namespace {

struct WriteFormattedXml {
  std::ostream& mOStr;
  int mIndent;
  WriteFormattedXml( std::ostream& ostr, int indent ) :
    mOStr( ostr ),
    mIndent( indent ) {}
  void operator()( RefPtr< XmlObject > obj ) {
    writeIndent( mOStr, *obj, mIndent );
  }
};

} // namespace anon

XmlOutputStream::XmlOutputStream( std::ostream& output ) :
  mOutput( output ),
  mContextStack() {
  output << "<?xml version='1.0'?>\n";
}

XmlOutputStream::~XmlOutputStream() {
  closeStream();
}

void XmlOutputStream::openContext( RefPtr< XmlObject > obj ) {
  // write the header and body of the input object first.  That way the context
  // isn't modified if this operation fails.
  obj->printHeader( mOutput, mContextStack.size() );
  obj->printTextContent( mOutput, mContextStack.size() );

  // now push the object onto the context stack.
  mContextStack.push( obj );

  if( obj->hasChildren() ) {
    // write the complete body of all but the final child to the stream
    XmlObject::ChildIterator finalChild = obj->endChildren();
    --finalChild;
    std::for_each( obj->beginChildren(), finalChild, 
      WriteFormattedXml( mOutput, mContextStack.size() ) );

    // open a context for my final child
    openContext( *finalChild );
  }
}

void XmlOutputStream::writeObject( RefPtr< XmlObject > obj ) {
  WriteFormattedXml write( mOutput, mContextStack.size() );
  write( obj );
}

void XmlOutputStream::closeCurrentContext() {
  assert( !mContextStack.empty() );

  // Save a reference to the current top of the stack.
  RefPtr< XmlObject > top = mContextStack.top();

  // pop the stack, we follow the reverse order of openContext so that the
  // indentation level is managed by the size of the stack.
  mContextStack.pop();

  // now print the footer of the closed object to the stream.
  top->printFooter( mOutput, mContextStack.size() );
}

void XmlOutputStream::closeStream() {
  while ( !mContextStack.empty() ) {
    closeCurrentContext();
  }
}

} // namespace xdm

