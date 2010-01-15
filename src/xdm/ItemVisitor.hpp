#ifndef xdm_ItemVisitor_hpp
#define xdm_ItemVisitor_hpp

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class Item;

class ItemVisitor {
public:

  ItemVisitor();
  virtual ~ItemVisitor();

  void traverse( Item& item );
  virtual void apply( Item& item );
};

/// Convenience function to apply an ItemVisitor to an Item.  To apply a
/// visitor, users should *never* call ItemVisitor::apply.  Instead, call
/// Item::accept( ItemVisitor ) or this helper function.  
/// This is an implementation of double dispatch. The Item class will call 
/// apply with the correct type information.
void applyVisitor( ItemVisitor& iv, Item& item );


XDM_NAMESPACE_END

#endif // xdm_ItemVisitor_hpp

