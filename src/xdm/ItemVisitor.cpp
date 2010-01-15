//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/ItemVisitor.hpp>

#include <xdm/CompositeDataItem.hpp>
#include <xdm/DataItem.hpp>
#include <xdm/UniformDataItem.hpp>

XDM_NAMESPACE_BEGIN

ItemVisitor::ItemVisitor() {
}

ItemVisitor::~ItemVisitor() {
}

void ItemVisitor::apply( Item& item ) {
  traverse( item );
}

void ItemVisitor::apply( DataItem& item ) {
  apply( static_cast< Item& >( item ) );
}

void ItemVisitor::apply( CompositeDataItem& item ) {
  apply( static_cast< DataItem& >( item ) );
}

void ItemVisitor::apply( UniformDataItem& item ) {
  apply( static_cast< DataItem& >( item ) );
}

XDM_NAMESPACE_END

