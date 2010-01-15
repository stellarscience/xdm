//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmComm_ReceiveBufferArray_hpp
#define xdmComm_ReceiveBufferArray_hpp

#include <xdm/VectorStructuredArray.hpp>

#include <vector>

#include <xdmComm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// StructuredArray for receiving off core data. The data is held internally
/// as a byte array. The data's type is determined by the type specified via
/// the primitiveType value stored internally within the class.
class ReceiveBufferArray : public xdm::StructuredArray {
  std::vector< char > mBuffer;
  xdm::primitiveType::Value mType;
  size_t mSize;

public:
  /// Construct allocating the given number of bytes for the buffer.
  /// @post The ReceiveBufferArray is ready to receive messages up to bufferSize
  /// length in bytes.
  /// @param bufferSize The size of the buffer to use in bytes.
  ReceiveBufferArray( size_t bufferSize ) :
    mBuffer( bufferSize ),
    mType(),
    mSize() {
  }
  virtual ~ReceiveBufferArray() {}

  //-- StructuredArray Query Interface --//

  virtual xdm::primitiveType::Value dataType() const { return mType; }
  virtual size_t elementSize() const { return xdm::typeSize( mType ); }
  virtual size_t size() const { return mSize; }
  virtual const void* data() const { return mBuffer.data(); }

  //-- Accessors for describing type and size information --//

  /// Set the type of value held within the array's buffer.
  void setDataType( const xdm::primitiveType::Value& type ) {
    mType = type;
  }

  /// Set the number of elements held withing the array's buffer.
  void setSize( size_t size ) {
    mSize = size;
  }

  //-- Safe Buffer Access --//
  char* buffer() { return mBuffer.data(); }
  const char* buffer() const { return mBuffer.data(); }

};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_ReceiveBufferArray_hpp
