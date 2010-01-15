//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------

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

