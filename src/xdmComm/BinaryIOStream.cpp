//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
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

