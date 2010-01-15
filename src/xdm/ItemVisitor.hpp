//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_ItemVisitor_hpp
#define xdm_ItemVisitor_hpp

#include <xdm/Item.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

// forward declarations of classes the visitor will operate on
class CompositeDataItem;
class DataItem;
class UniformDataItem;

/// Class to represent type safe operations on the data tree.  Subclasses of
/// ItemVisitor should reimplement the apply functions for the core xdm objects
/// they wish to operate on.  NOTE:  To apply a visitor to an Item, call
/// Item::accept(visitor), DO NOT call ItemVisitor::apply(item).  This is to
/// ensure the correct type information is passed on to the visitor.
class ItemVisitor : public virtual ReferencedObject {
public:
  ItemVisitor();
  virtual ~ItemVisitor();

  //-- Apply functions --//
  virtual void apply( Item& item );
  virtual void apply( DataItem& item );
  virtual void apply( CompositeDataItem& item );
  virtual void apply( UniformDataItem& item );

  /// Traverse an Item.  Implementors should call this to allow the Item's
  /// subclass to determine how traversal must occur.
  inline void traverse( Item& item ) {
    item.traverse( *this );
  }
};

/// Convenience functor for applying a visitor to an Item.
class ApplyVisitor {
public:
  /// Pass in a reference to the visitor to apply.
  ApplyVisitor( ItemVisitor& iv ) : mIV( iv ) {}
  /// Copy constructor
  ApplyVisitor( const ApplyVisitor& av ) : mIV( av.mIV ) {}

  void operator()( Item* item ) { item->accept( mIV ); }
  void operator()( RefPtr< Item > item ) { item->accept( mIV ); };

private:
  ApplyVisitor& operator=( const ApplyVisitor& ) { return *this; }
  ItemVisitor& mIV;
};

XDM_NAMESPACE_END

#endif // xdm_ItemVisitor_hpp

