//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmComm_BinaryIOStream_hpp
#define xdmComm_BinaryIOStream_hpp

#include <xdmComm/BinaryIStream.hpp>
#include <xdmComm/BinaryOStream.hpp>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

class BinaryIOStream : 
  public BinaryIStream,
  public BinaryOStream {
public:
  BinaryIOStream( BinaryStreamBuffer* buf );
  virtual ~BinaryIOStream();
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_BinaryIOStream_hpp

