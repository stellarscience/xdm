
#include <xdm/StructuredArray.hpp>

#include <numeric>
#include <vector>

XDM_NAMESPACE_BEGIN

StructuredArray::StructuredArray( 
  const primitiveType::Value& type,
  size_t arrayElementSize,
  void* data,
  const DataShape<>& shape ) :
  mType( type ),
  mArrayElementSize( arrayElementSize ),
  mData( data ),
  mShape( shape ) {
}

StructuredArray::~StructuredArray() {
}

size_t StructuredArray::elementSize() const {
  return mArrayElementSize;
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

XDM_NAMESPACE_END

