#ifndef xdmComm_BinaryOStream_hpp
#define xdmComm_BinaryOStream_hpp

#include <xdmComm/BinaryIosBase.hpp>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

class BinaryOStream : virtual public BinaryIosBase {
public:
  
  /// Constructor takes a pointer to a stream buffer.
  BinaryOStream( BinaryStreamBuffer* buf ); 
  virtual ~BinaryOStream();

  /// Put a single character to the stream.
  BinaryOStream& put( char c );
  /// Put n characters to the stream.
  BinaryOStream& write( const char* p, size_t n );

  //-- Insertion Operators --//

  BinaryOStream& operator<<( short n );
  BinaryOStream& operator<<( int n );
  BinaryOStream& operator<<( long n );

  BinaryOStream& operator<<( unsigned short n );
  BinaryOStream& operator<<( unsigned int n );
  BinaryOStream& operator<<( unsigned long n );

  BinaryOStream& operator<<( float n );
  BinaryOStream& operator<<( double n );
  BinaryOStream& operator<<( long double n );
  
  BinaryOStream& operator<<( bool n );

  BinaryOStream& flush();

  /// Insertion operator that takes a function pointer and invokes it on the
  /// stream.  This allows the stream to support manipulators.
  BinaryOStream& operator<<( BinaryOStream& (*f)( BinaryOStream& ) ) {
    return f( *this );
  }
};

BinaryOStream& operator<<( BinaryOStream& ostr, char c );
BinaryOStream& operator<<( BinaryOStream& ostr, unsigned char c );

/// Stream manipulator that flushes the stream.
inline BinaryOStream& flush( BinaryOStream& ostr ) { return ostr.flush(); }

XDM_COMM_NAMESPACE_END

#endif // xdmComm_BinaryOStream_hpp

