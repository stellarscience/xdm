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

XDM_NAMESPACE_END

#endif // xdm_ItemVisitor_hpp

