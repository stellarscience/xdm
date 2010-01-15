#ifndef xdm_Domain_hpp
#define xdm_Domain_hpp

#include <xdm/Item.hpp>

#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class Grid;

class Domain : public Item {
public:
  
  Domain();
  virtual ~Domain();

  bool appendGrid( Grid* grid );

private:
};

XDM_NAMESPACE_END

#endif // xdm_Domain_hpp

