#ifndef xdmComm_BinaryIStream_hpp
#define xdmComm_BinaryIStream_hpp

#include <xdmComm/BinaryIosBase.hpp>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

class BinaryIStream : virtual public BinaryIosBase {
public:
  
  /// Constructor takes a pointer to a stream buffer.
  BinaryIStream( BinaryStreamBuffer* buf ); 
  virtual ~BinaryIStream();

  /// Read a single character from the stream.
  BinaryIStream& get( char& c );
  /// Read a block of n characters from the stream.
  BinaryIStream& read( char* const p, size_t n );

  //-- Exraction Operators --//

  BinaryIStream& operator>>( short& n );
  BinaryIStream& operator>>( int& n );
  BinaryIStream& operator>>( long& n );

  BinaryIStream& operator>>( unsigned short& n );
  BinaryIStream& operator>>( unsigned int& n );
  BinaryIStream& operator>>( unsigned long& n );

  BinaryIStream& operator>>( float& n );
  BinaryIStream& operator>>( double& n );
  BinaryIStream& operator>>( long double& n );
  
  BinaryIStream& operator>>( bool& n );

  BinaryIStream& sync();
};

BinaryIStream& operator>>( BinaryIStream& ostr, char& c );

XDM_COMM_NAMESPACE_END

#endif // xdmComm_BinaryIStream_hpp

