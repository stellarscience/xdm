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
#include <xdm/BinaryIStream.hpp>

#include <memory>

XDM_NAMESPACE_BEGIN

namespace {

// helper template to write a primitive type to a binary ostream.
template< typename T >
inline BinaryIStream& input( BinaryIStream& ostr, T& value ) {
  return ostr.read( reinterpret_cast< char* >( &value ), sizeof( T ) );
}

} // namespace anon

BinaryIStream::BinaryIStream( BinaryStreamBuffer* buf ) :
  BinaryIosBase( buf ) {
}

BinaryIStream::~BinaryIStream() {
}

int BinaryIStream::get() {
  return rdbuf()->sgetc();
}

BinaryIStream& BinaryIStream::get( char& c ) {
  c = rdbuf()->sgetc();
  return *this;
}

BinaryIStream& BinaryIStream::read( char* const p, size_t n ) {
  rdbuf()->sgetn( p, n );
  return *this;
}

BinaryIStream& BinaryIStream::operator>>( short& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::operator>>( int& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::operator>>( long& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::operator>>( unsigned short& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::operator>>( unsigned int& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::operator>>( unsigned long& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::operator>>( float& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::operator>>( double& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::operator>>( long double& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::operator>>( bool& n ) {
  return input( *this, n );
}

BinaryIStream& BinaryIStream::sync() {
  rdbuf()->pubsync();
  return *this;
}

BinaryIStream& operator>>( BinaryIStream& ostr, char& c ) {
  return ostr.get( c );
}

XDM_NAMESPACE_END

