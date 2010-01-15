//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmComm/BinaryIStream.hpp>

#include <memory>

XDM_COMM_NAMESPACE_BEGIN

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

XDM_COMM_NAMESPACE_END

