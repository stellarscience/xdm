#include <xdmGrid/UniformGrid.hpp>

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/Topology.hpp>

#include <xdm/ItemVisitor.hpp>

#include <algorithm>

XDM_GRID_NAMESPACE_BEGIN

UniformGrid::UniformGrid() :
  Grid() {
}

UniformGrid::~UniformGrid() {
}

void UniformGrid::setGeometry( Geometry* geo ) {
  mGeometry = geo;
}

const Geometry* UniformGrid::geometry() const {
  return mGeometry;
}

Geometry* UniformGrid::geometry() {
  return mGeometry;
}

void UniformGrid::setTopology( Topology* topo ) {
  mTopology = topo;
}

Topology* UniformGrid::topology() {
  return mTopology;
}

const Topology* UniformGrid::topology() const {
  return mTopology;
}

void UniformGrid::traverse( xdm::ItemVisitor& iv ) {
  Grid::traverse( iv );
  // apply the visitor to my internal geometry and topology items
  mTopology->accept( iv );
  mGeometry->accept( iv );
  std::for_each( 
    xdm::begin< Attribute >( *this ), 
    xdm::end< Attribute >( *this ), 
    xdm::ApplyVisitor( iv ) );
}

void UniformGrid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Grid::writeMetadata( xml );

  // write Uniform grid type
  xml.setAttribute( "GridType", "Uniform" );
}

XDM_GRID_NAMESPACE_END

