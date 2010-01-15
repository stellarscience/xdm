#ifndef xdmGrid_UnstructuredTopology_hpp
#define xdmGrid_UnstructuredTopology_hpp

#include <xdmGrid/Topology.hpp>

#include <xdm/DataShape.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Base class for all unstructured topologies. If the connectivity is different
/// from the standard, an order may be specified.
class UnstructuredTopology : public Topology {
public:
  UnstructuredTopology();
  virtual ~UnstructuredTopology();

  XDM_META_ITEM( UnstructuredTopology );

  /// Set the number of elements in the topology.
  void setNumberOfElements( unsigned int numberOfElements );
  /// Get the number of elements in the topology.
  unsigned int numberOfElements() const;

  /// Define how many nodes make up a single element.
  void setNodesPerElement( unsigned int nodesPerElement );
  /// Determine how many nodes make up a single element.
  unsigned int nodesPerElement() const;

  /// Set the connectivity values to the input DataItem. If the connectivity
  /// is not specified, then there is a default connectivity determined by the
  /// topology type.
  void setConnectivity( xdm::DataItem* connectivity );

  virtual void traverse( xdm::ItemVisitor& iv );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  unsigned int mNumberOfElements;
  unsigned int mNodesPerElement;
  xdm::RefPtr< xdm::DataItem > mConnectivity;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_UnstructuredTopology_hpp

