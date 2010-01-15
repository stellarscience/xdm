
#include <xdm/StructuredArray.hpp>

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

size_t StructuredArray::dataSize() const {
  size_t size = 1;
  for ( int i = 0; i < mShape.rank(); ++i ) {
    size *= mShape[i];
  }
  return size;
}

void* StructuredArray::data() {
  return mData;
}

const void* StructuredArray::data() const {
  return mData;
}

XDM_NAMESPACE_END

