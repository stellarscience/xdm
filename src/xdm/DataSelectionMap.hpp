//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_DataSelectionMap_hpp
#define xdm_DataSelectionMap_hpp

#include <xdm/RefPtr.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class DataSelection;

/// Definition of a mapping from one selection to another.
class DataSelectionMap {
private:
  RefPtr< DataSelection > mDomain;
  RefPtr< DataSelection > mRange;

public:
  /// Default constructor selects all points in both domain and range spaces.
  DataSelectionMap();
  /// Initialize the domain and range selections.
  DataSelectionMap( DataSelection* domain, DataSelection* range );
  ~DataSelectionMap();

  const DataSelection* domain() const;
  void setDomain( DataSelection* domain );

  const DataSelection* range() const;
  void setRange( DataSelection* range );
};

XDM_NAMESPACE_END

#endif // xdm_DataSelectionMap_hpp
