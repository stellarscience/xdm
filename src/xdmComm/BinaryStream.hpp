#ifndef xdmComm_BinaryStream_hpp
#define xdmComm_BinaryStream_hpp

#include <xdm/PrimitiveType.hpp>

#include <complex>

namespace xdmComm {

/// Stream that outputs objects to a memory buffer.  Accepts iostream like
/// inserters and extractors (<< and >>).
class BinaryStream {
public:

  typedef std::size_t size_type;

  BinaryStream( size_type buffer_size );
  ~BinaryStream();

  /// Get the location of the put pointer.
  char* tellp() const;
  /// Get the location of the get pointer.
  char* tellg() const;

  /// Put a value with a given size into the buffer.
  void write( const char* c, size_type n );
  /// Get a value with a given size from the buffer.
  void read( char* c, size_type n );
  
	//-- Inserters for primitive types --//
	
	BinaryStream& operator<<( const char& v );
	BinaryStream& operator<<( const short& v );
	BinaryStream& operator<<( const int& v );
	BinaryStream& operator<<( const long int& v );
	BinaryStream& operator<<( const unsigned char& v );
	BinaryStream& operator<<( const unsigned short& v );
	BinaryStream& operator<<( const unsigned int& v );
	BinaryStream& operator<<( const long unsigned int& v );
	BinaryStream& operator<<( const float& v );
	BinaryStream& operator<<( const double& v );
	  
	//-- Extractors for primitive types --//
	
	BinaryStream& operator>>( char& v );
	BinaryStream& operator>>( short& v );
	BinaryStream& operator>>( int& v );
	BinaryStream& operator>>( long int& v );
	BinaryStream& operator>>( unsigned char& v );
	BinaryStream& operator>>( unsigned short& v );
	BinaryStream& operator>>( unsigned int& v );
	BinaryStream& operator>>( long unsigned int& v );
	BinaryStream& operator>>( float& v );
	BinaryStream& operator>>( double& v );

private:
  size_type m_size;
  char* m_buffer;
  char* m_put;
  char* m_get;
}


template< typename T >
BinaryStream& operator<<( BinaryStream& ostr, const std::complex< T >& v );
BinaryStream& operator<<( BinaryStream& ostr, const std::string& v );

template< typename T >
BinaryStream& operator>>( BinaryStream& istr, std::complex< T >& v );
BinaryStream& operator>>( BinaryStream& istr, std::string& v );

} // namespace XdmComm

#endif // xdmComm_BinaryStream_hpp

