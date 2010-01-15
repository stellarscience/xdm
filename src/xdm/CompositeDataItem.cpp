//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/CompositeDataItem.hpp>

#include <algorithm>

#include <cassert>

XDM_NAMESPACE_BEGIN

CompositeDataItem::CompositeDataItem() :
  DataItem(),
  ObjectCompositionMixin< DataItem >() {
}

CompositeDataItem::CompositeDataItem( unsigned int n ) :
  DataItem(),
  ObjectCompositionMixin< DataItem >( n ) {
}

CompositeDataItem::~CompositeDataItem() {
}

void CompositeDataItem::traverse( ItemVisitor& iv ) {
  std::for_each( begin(), end(), ApplyVisitor( iv ) );
}

void CompositeDataItem::writeMetadata( XmlMetadataWrapper& xml ) {
  DataItem::writeMetadata( xml );

  // write my ItemType as a collection.
  xml.setAttribute( "ItemType", "Tree" );
}

XDM_NAMESPACE_END

