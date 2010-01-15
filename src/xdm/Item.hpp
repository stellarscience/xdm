#ifndef xdm_Item_hpp
#define xdm_Item_hpp

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class ItemVisitor;

class Item {
public:

  Item();
  virtual ~Item();

  virtual void accept( ItemVisitor& );
  virtual void traverse( ItemVisitor& );
};

XDM_NAMESPACE_END

#endif // xdmf_Item_hpp

