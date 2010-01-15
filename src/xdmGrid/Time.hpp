//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmGrid_Time_hpp
#define xdmGrid_Time_hpp

#include <xdm/DataItem.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Time : public xdm::Item {
public:
  explicit Time( double value = 0.0 );
  virtual ~Time();

  /// Set the time value for this item.
  void setValue( double value );
  double value() const;

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  double mValue;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Time_hpp

