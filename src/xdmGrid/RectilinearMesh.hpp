#ifndef xdmGrid_RectilinearMesh_hpp
#define xdmGrid_RectilinearMesh_hpp

#include <xdmGrid/StructuredTopology.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Mesh in which axes are mutually orthogonal with variable spacing between
/// nodes.
class RectilinearMesh : public StructuredTopology {
public:
  RectilinearMesh();
  virtual ~RectilinearMesh();

  XDM_META_ITEM( RectilinearMesh );
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_RectilinearMesh_hpp
