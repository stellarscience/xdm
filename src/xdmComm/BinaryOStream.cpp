#include <xdmComm/BinaryOStream.hpp>

#include <memory>

XDM_COMM_NAMESPACE_BEGIN

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

XDM_COMM_NAMESPACE_END

