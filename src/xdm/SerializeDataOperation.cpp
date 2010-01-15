//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/SerializeDataOperation.hpp>

#include <xdm/UniformDataItem.hpp>

XDM_NAMESPACE_BEGIN

SerializeDataOperation::SerializeDataOperation() {
}

SerializeDataOperation::~SerializeDataOperation() {
}

void SerializeDataOperation::apply( UniformDataItem& udi ) {
  udi.serializeData();
}

XDM_NAMESPACE_END

