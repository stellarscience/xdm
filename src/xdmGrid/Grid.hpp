#ifndef xdm_Grid_hpp
#define xdm_Grid_hpp

#include <xdm/Item.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Grid : public xdm::Item {
public:
  Grid();
  virtual ~Grid();
};

XDM_GRID_NAMESPACE_END

#endif // xdm_Grid_hpp

