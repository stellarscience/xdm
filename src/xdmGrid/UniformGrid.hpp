#ifndef xdmGrid_UniformGrid_hpp
#define xdmGrid_UniformGrid_hpp

#include <xdmGrid/Grid.hpp>

#include <xdm/RefPtr.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Geometry;
class Topology;

/// Grid type containing the actual geometry and topology for a grid.  This is a
/// terminal grid node that contains the geometric and topological properties of
/// a Grid.
class UniformGrid : public Grid {
public:
  UniformGrid();
  virtual ~UniformGrid();

  void setGeometry( Geometry* geo );
  Geometry* geometry();
  const Geometry* geometry() const;

  void setTopology( Topology* topo );
  Topology* topology();
  const Topology* topology() const;

private:
  xdm::RefPtr< Geometry > mGeometry;
  xdm::RefPtr< Topology > mTopology;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_UniformGrid_hpp

