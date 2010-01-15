//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_DataItem_hpp
#define xdm_DataItem_hpp

#include <xdm/Item.hpp>
#include <xdm/ItemVisitor.hpp>
#include <xdm/PrimitiveType.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// A DataItem is a class that holds large datasets.
class DataItem : public Item {
public:
  DataItem();
  virtual ~DataItem();

  XDM_META_ITEM( DataItem );

  virtual void writeMetadata( XmlMetadataWrapper& xml );
};

XDM_NAMESPACE_END

#endif // xdm_DataItem_hpp

