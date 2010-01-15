#ifndef xdm_Group_hpp
#define xdm_Group_hpp

#include <xdm/Item.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <vector>

XDM_NAMESPACE_BEGIN

typedef std::vector< Item* > ItemList;

class Group : public Item {

  Group();
  virtual ~Group();

  //-- Item Interface --//
  virtual void accept( ItemVisitor& iv );
  virtual void traverse( ItemVisitor& iv );

private:
  ItemList children;
};

XDM_NAMESPACE_END

#endif // xdm_Group_hpp

