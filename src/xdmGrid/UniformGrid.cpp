#include <xdmGrid/UniformGrid.hpp>

#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/Topology.hpp>

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

XDM_GRID_NAMESPACE_END

