//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_Grid_hpp
#define xdm_Grid_hpp

#include <xdm/Item.hpp>
#include <xdm/ItemVisitor.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Time;

class Grid : public xdm::Item {
public:
  Grid();
  virtual ~Grid();

  XDM_META_ITEM( Grid );

  Time* time();
  const Time* time() const;
  void setTime( Time* time );

  virtual void traverse( xdm::ItemVisitor& iv );

  /// Write grid metadata.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

public:
  xdm::RefPtr< Time > mTime;
};

XDM_GRID_NAMESPACE_END

#endif // xdm_Grid_hpp

