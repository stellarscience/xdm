#ifndef xdmGrid_Topology_hpp
#define xdmGrid_Topology_hpp

#include <xdm/CompositeDataItem.hpp>
#include <xdm/DataShape.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Base class for topological descriptions of a grid.  The topological
/// properties of a grid are those which are invariant under rotations,
/// translations, and scale.  Connectivity is the most important topological
/// property.
///
/// A Grid consists of elements (or cells) and nodes.
class Topology : public xdm::CompositeDataItem {
public:
  Topology();
  virtual ~Topology();

  XDM_META_ITEM( Topology );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Topology_hpp

