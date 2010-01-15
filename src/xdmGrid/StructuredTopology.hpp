#ifndef xdmGrid_StructuredTopology_hpp
#define xdmGrid_StructuredTopology_hpp

#include <xdmGrid/Topology.hpp>

#include <xdm/DataShape.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Grid topology for which connectivity is implicit.  Namely, node i is
/// connected to node i+1.  Examples of structured topologies are grid
/// topologies in two or three dimensions.
class StructuredTopology : public Topology {
public:
  StructuredTopology();
  virtual ~StructuredTopology();

  XDM_META_ITEM( StructuredTopology );

  /// Set the shape defined by the cells in the topology.  This shape should
  /// correspond to the arrangement of cells in n-dimensional space.
  void setShape( const xdm::DataShape<>& shape );
  /// Get the shape defined by the cells in the topology.
  const xdm::DataShape<>& shape() const;

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );
private:
  xdm::DataShape<> mShape;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_StructuredTopology_hpp

