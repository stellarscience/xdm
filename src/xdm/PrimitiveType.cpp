//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/PrimitiveType.hpp>

#include <map>

XDM_NAMESPACE_BEGIN

size_t typeSize( primitiveType::Value type ) {
  static size_t sizes[] = {
    sizeof(char),
    sizeof(short),
    sizeof(int),
    sizeof(long int),
    sizeof(unsigned char),
    sizeof(unsigned short),
    sizeof(unsigned int),
    sizeof(unsigned long int),
    sizeof(float),
    sizeof(double)
  };
  return sizes[type];
}

XDM_NAMESPACE_END

