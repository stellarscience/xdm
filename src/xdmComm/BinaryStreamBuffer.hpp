#ifndef xdmComm_BinaryStreamBuffer_hpp
#define xdmComm_BinaryStreamBuffer_hpp

#include <memory>

#include <cassert>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Class to handle reading and writing binary data to/from a buffer.  The I/O
/// functions in this buffer handle the management of a buffer pointer so that
/// items may be read/written incrementally to a stream.  The interface of this
/// class is designed to mimic that of a std::streambuf so that we may use that
/// someday.
///
/// This class does not fill and output as a std::streambuf would.  An explicit
/// call to sync() must be called to flush the buffer occasionally, or it will
/// fill up.  Attempts to put new data into a full buffer will result in failed
/// assertions.
template< typename AllocT = std::allocator<char> >
class BasicBinaryStreamBuffer {
private:
  size_t mSize;
  char* mData;
  char* mLocation;

  AllocT mAlloc;

public:

  BasicBinaryStreamBuffer( size_t size ) :
    mSize( size ),
    mData( mAlloc.allocate( size ) ),
    mLocation( mData ) {}

  virtual ~BasicBinaryStreamBuffer() {
    mAlloc.deallocate( mData, mSize );
  }

  /// Put a single character into the buffer at the current location.
  void sputc( char c ) { 
    assert( mLocation + 1 < mData + mSize );
    *mLocation++ = c; 
  }

  /// Put a character sequence into the buffer at the current location. 
  void sputn( const char* in, size_t n ) {
    assert( mLocation + n < mData + mSize );
    std::uninitialized_copy( in, in + n, mLocation );
    mLocation += n;
  }

  /// Get a single character from the buffer at the current location.
  char sgetc() {
    assert( mLocation < mData + mSize );
    return *mLocation++;
  }

  /// Get a sequence of characters from the buffer at the current location.
  void sgetn( char* out, size_t n ) {
    assert( mLocation + n < mData + mSize );
    std::uninitialized_copy( mLocation, mLocation + n, out );
    mLocation += n;
  }

  /// Get the allocated size of the buffer.
  size_t size() const {
    return mSize;
  }

  char* pointer() {
    return mData;
  }

  /// Reset the location pointer to the beginning of the buffer.
  void pubsync() {
    sync();
  }

protected:
  virtual void sync() {
    mLocation = mData;
  }
};

/// BinaryStreamBuffer with a standard allocator.
typedef BasicBinaryStreamBuffer<> BinaryStreamBuffer;

XDM_COMM_NAMESPACE_END

#endif // xdmComm_BinaryStreamBuffer_hpp

