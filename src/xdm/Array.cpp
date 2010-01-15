
#include <xdm/Array.hpp>

#include <vector>

XDM_NAMESPACE_BEGIN

Array::Array( const DataShape<>& shape ) : 
  mData( shape[0] ),
  mShape( shape ) {
}

Array::~Array() {
}

size_t Array::size() const {
  return mData.size();
}

float& Array::at( size_t i ) {
  return mData[i];
}

const float& Array::at( size_t i ) const {
  return mData[i];
}

float* Array::data() {
  return &(mData[0]);
}

const float* Array::data() const {
  return &(mData[0]);
}

XDM_NAMESPACE_END

