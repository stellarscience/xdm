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
#include <xdm/BinaryOStream.hpp>

#include <memory>

XDM_NAMESPACE_BEGIN

namespace {

// helper template to write a primitive type to a binary ostream.
template< typename T >
inline BinaryOStream& output( BinaryOStream& ostr, T value ) {
  return ostr.write( reinterpret_cast< char* >( &value ), sizeof( T ) );
}

} // namespace anon

BinaryOStream::BinaryOStream( BinaryStreamBuffer* buf ) :
  BinaryIosBase( buf ) {
}

BinaryOStream::~BinaryOStream() {
}

BinaryOStream& BinaryOStream::put( char c ) {
  rdbuf()->sputc( c );
  return *this;
}

BinaryOStream& BinaryOStream::write( const char* p, size_t n ) {
  rdbuf()->sputn( p, n );
  return *this;
}

BinaryOStream& BinaryOStream::operator<<( short n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::operator<<( int n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::operator<<( long n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::operator<<( unsigned short n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::operator<<( unsigned int n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::operator<<( unsigned long n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::operator<<( float n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::operator<<( double n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::operator<<( long double n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::operator<<( bool n ) {
  return output( *this, n );
}

BinaryOStream& BinaryOStream::flush() {
  rdbuf()->pubsync();
  return *this;
}

BinaryOStream& operator<<( BinaryOStream& ostr, char c ) {
  return ostr.put( c );
}

BinaryOStream& operator<<( BinaryOStream& ostr, unsigned char c ) {
  return ostr.put( c );
}

XDM_NAMESPACE_END

