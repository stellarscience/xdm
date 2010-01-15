//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmGrid_Domain_hpp
#define xdmGrid_Domain_hpp

#include <xdmGrid/Grid.hpp>

#include <xdm/DataItem.hpp>
#include <xdm/Item.hpp>
#include <xdm/ObjectCompositionMixin.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Domain : 
  public xdm::Item,
  public xdm::ObjectCompositionMixin< xdm::DataItem >,
  public xdm::ObjectCompositionMixin< Grid > {
public:

  XDM_META_ITEM( Domain );

  /// Add a grid to this Domain.
  void addGrid( Grid* g ) { xdm::appendChild< Grid >( *this, g ); }

  virtual void traverse( xdm::ItemVisitor& iv );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Domain_hpp

