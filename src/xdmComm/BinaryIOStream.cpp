#include <xdmComm/BinaryIOStream.hpp>

XDM_COMM_NAMESPACE_BEGIN

BinaryIOStream::BinaryIOStream( BinaryStreamBuffer* buf ) :
  BinaryIosBase( buf ),
  BinaryIStream( buf ),
  BinaryOStream( buf ) {
}

BinaryIOStream::~BinaryIOStream() {
}

XDM_COMM_NAMESPACE_END

