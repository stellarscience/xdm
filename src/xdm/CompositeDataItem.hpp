//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_CompositeDataItem_hpp
#define xdm_CompositeDataItem_hpp

#include <xdm/DataItem.hpp>
#include <xdm/ObjectCompositionMixin.hpp>
#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Class that groups multiple Data nodes together.  CompositeDataItem is a Data
/// type that can hold child Data types.  It is used to construct a large
/// dataset from multiple smaller datasets.
class CompositeDataItem : 
  public DataItem,
  public ObjectCompositionMixin< DataItem > {
public:
  CompositeDataItem();
  
  /// Constructor takes the number of children to hold.
  explicit CompositeDataItem( unsigned int n );

  virtual ~CompositeDataItem();

  XDM_META_ITEM( CompositeDataItem );

  /// Redefinition of visitor traversal from Item.
  virtual void traverse( ItemVisitor& iv );

  /// Write the metadata for a collection of dataitems.
  virtual void writeMetadata( XmlMetadataWrapper& xml );
};

XDM_NAMESPACE_END

#endif // xdm_CompositeDataItem_hpp

