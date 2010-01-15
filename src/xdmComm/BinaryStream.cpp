
#include <xdmComm/BinaryStream.hpp>

#include <cstdlib>

namespace {
  /// Helper template to place a simple type in a char array.
  template< typename T >
  insertPOD( char* loc, const T& v ) {
    memcpy( loc, &v, sizeof( T );
  }
  /// Helper template to get a simple type from a char array.
  template< typename T >
  extractPOD( char* loc, T& v ) {
    memcpy( &v, loc, sizeof( T ) );
  }
} // namespace anon

namespace xdmComm {

BinaryStream::BinaryStream( size_type buffer_size ) :
  m_size( buffer_size ),
  m_buffer( new char[ buffer_size ] ),
  m_put( m_buffer ),
  m_get( m_buffer )
{
}

BinaryStream::~BinaryStream() {
  delete [] m_buffer;
}

char* tellp() const {
  return m_put;
}

char* tellg() const {
  return m_get;
}



} // namespace xdmComm

