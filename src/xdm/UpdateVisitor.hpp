//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_UpdateVisitor_hpp
#define xdm_UpdateVisitor_hpp

#include <xdm/ItemVisitor.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Visitor class that traverses the tree invoking update callbacks.  This
/// allows client applications to define application specific update behavior
/// but pass the management of invoking that behavior to the library.
class UpdateVisitor : public xdm::ItemVisitor {
public:

  virtual void apply( UniformDataItem& item );

};

XDM_NAMESPACE_END

#endif // xdm_UpdateVisitor_hpp

