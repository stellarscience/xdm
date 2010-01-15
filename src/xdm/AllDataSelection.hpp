//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_AllDataSelection_hpp
#define xdm_AllDataSelection_hpp

#include <xdm/DataSelection.hpp>
#include <xdm/DataSelectionVisitor.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// DataSelection that simply selects all data.
class AllDataSelection : public DataSelection {
public:
  AllDataSelection() {}
  virtual ~AllDataSelection() {}
  virtual void accept( DataSelectionVisitor& v ) const { v.apply(*this); }
};

inline bool operator==( 
  const AllDataSelection& lhs, 
  const AllDataSelection& rhs ) {
  return true;
}

XDM_NAMESPACE_END

#endif // xdm_AllDataSelection_hpp

