#ifndef xdm_Item_hpp
#define xdm_Item_hpp

#include <xdm/NamespaceMacro.hpp>

#include <string>
#include <vector>

XDM_NAMESPACE_BEGIN

class ItemVisitor;

class Item {
public:
  Item();
  virtual ~Item();

  inline void setName( const std::string& name ) {
    mName = name;
  }
  inline const std::string& name() const {
    return mName;
  }

  virtual void accept( ItemVisitor& iv );
  virtual void traverse( ItemVisitor& iv );

  unsigned int childCount() const;

protected:
  bool appendChild( Item* child );

private:
  std::string mName;
  typedef std::vector< Item* > ItemList;
  ItemList mChildren;
};

XDM_NAMESPACE_END

#endif // xdmf_Item_hpp

