
#include <xdm/StructuredArray.hpp>

#include <numeric>
#include <vector>

XDM_NAMESPACE_BEGIN

StructuredArray::StructuredArray() {
}

StructuredArray::~StructuredArray() {
}

size_t StructuredArray::memorySize() const {
  return elementSize() * size();
}

XDM_NAMESPACE_END

