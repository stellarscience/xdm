//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/DataItem.hpp>

XDM_NAMESPACE_BEGIN

DataItem::DataItem() {
}

DataItem::~DataItem() {
}

void DataItem::writeMetadata( XmlMetadataWrapper& xml ) {
  Item::writeMetadata( xml );
  xml.setTag( "DataItem" );
}

XDM_NAMESPACE_END

