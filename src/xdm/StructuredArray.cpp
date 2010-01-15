//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------

#include <xdm/StructuredArray.hpp>

#include <numeric>
#include <vector>

XDM_NAMESPACE_BEGIN

StructuredArray::StructuredArray( 
  const primitiveType::Value& type,
  void* data,
  const DataShape<>& shape ) :
  mType( type ),
  mData( data ),
  mShape( shape ) {
}

StructuredArray::~StructuredArray() {
}

size_t StructuredArray::elementSize() const {
  return typeSize( mType );
}

size_t StructuredArray::size() const {
  return std::accumulate( mShape.begin(), mShape.end(),
    1, std::multiplies< size_t >() );
}

void* StructuredArray::data() {
  return mData;
}

const void* StructuredArray::data() const {
  return mData;
}

void StructuredArray::setData( void* data ) {
  mData = data;
}

XDM_NAMESPACE_END

