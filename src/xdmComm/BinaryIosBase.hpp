#ifndef xdmComm_BinaryIosBase_hpp
#define xdmComm_BinaryIosBase_hpp

#include <xdmComm/BinaryStreamBuffer.hpp>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Base class that manages access to an underlying stream buffer for streaming
/// input and output of binary serialized objects.
template< typename StreamBufT >
class BasicBinaryIosBase {
private:
  StreamBufT* mStreamBuffer;

  // non-copyable
  BasicBinaryIosBase( const BasicBinaryIosBase& );
  BasicBinaryIosBase& operator=( const BasicBinaryIosBase& );

public:

  typedef StreamBufT stream_buffer;

  /// The constructor takes a reference to the underlying stream buffer to use.
  /// The buffer's position pointer is reset to the beginning of the buffer upon
  /// construction.
  BasicBinaryIosBase( StreamBufT* buffer ) :
    mStreamBuffer( buffer ) {
    buffer->pubseekpos( 0 );
  }

  ~BasicBinaryIosBase() {}

  /// Get the mutable stream buffer.
  stream_buffer* rdbuf() { return mStreamBuffer; } 
};

typedef BasicBinaryIosBase< BinaryStreamBuffer > BinaryIosBase;

XDM_COMM_NAMESPACE_END

#endif // xdmComm_BinaryIosBase_hpp

