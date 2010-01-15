#include <xdmGrid/UnstructuredTopology.hpp>

#include <sstream>

XDM_GRID_NAMESPACE_BEGIN

UnstructuredTopology::UnstructuredTopology() :
  Topology(),
  mNumberOfElements(),
  mNodesPerElement(),
  mConnectivity() {
}

UnstructuredTopology::~UnstructuredTopology() {
}

void UnstructuredTopology::setNumberOfElements( unsigned int numberOfElements ) {
  mNumberOfElements = numberOfElements;
}

unsigned int UnstructuredTopology::numberOfElements() const {
  return mNumberOfElements;
}

void UnstructuredTopology::setNodesPerElement( unsigned int nodesPerElement ) {
  mNodesPerElement = nodesPerElement;
}

unsigned int UnstructuredTopology::nodesPerElement() const {
  return mNodesPerElement;
}

void UnstructuredTopology::setConnectivity( xdm::DataItem* connectivity ) {
  mConnectivity = connectivity;
}

void UnstructuredTopology::traverse( xdm::ItemVisitor& iv ) {
  if ( mConnectivity.valid() ) {
    mConnectivity->accept( iv );
  }
}

void UnstructuredTopology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Topology::writeMetadata( xml );

  // Write the number of elements
  xml.setAttribute( "NumberOfElements", mNumberOfElements );

  // Write the nodes per element
  xml.setAttribute( "NodesPerElement", mNodesPerElement );
}

XDM_GRID_NAMESPACE_END

