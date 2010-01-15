#ifndef xdm_Item_hpp
#define xdm_Item_hpp

#include <xdm/ReferencedObject.hpp>

#include <string>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Base class for all elements of the data hierarchy.
class Item : public ReferencedObject {
public:
  Item();
  virtual ~Item();

  void setName( const std::string& name );
  const std::string& name() const;

private:
  std::string mName;
};

XDM_NAMESPACE_END

#endif // xdm_Item_hpp

