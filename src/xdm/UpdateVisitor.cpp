//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdm/UpdateVisitor.hpp>

#include <xdm/Dataset.hpp>
#include <xdm/UniformDataItem.hpp>

XDM_NAMESPACE_BEGIN

void UpdateVisitor::apply( UniformDataItem& item ) {
  xdm::Dataset* itemDataset = item.dataset();
  if ( itemDataset ) {
    itemDataset->update();
  }
}

XDM_NAMESPACE_END

