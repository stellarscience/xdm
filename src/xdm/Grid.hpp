#ifndef xdm_Grid_hpp
#define xdm_Grid_hpp

#include <xdm/Item.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class Geometry;
class Topology;

class Grid : public Item {
public:

  Grid();
  virtual ~Grid();

  void setGeometry( Geometry* geo );
  Geometry* geometry();
  const Geometry* geometry() const;

  void setTopology( Topology* topo );
  Topology* topology();
  const Topology* topology() const;

  virtual void accept( ItemVisitor& iv );
  virtual void traverse( ItemVisitor& iv );
};

XDM_NAMESPACE_END

#endif // xdm_Grid_hpp

