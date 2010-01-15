//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmGrid/StructuredTopology.hpp>

#include <sstream>

XDM_GRID_NAMESPACE_BEGIN

StructuredTopology::StructuredTopology() :
  Topology(),
  mShape() {
}

StructuredTopology::~StructuredTopology() {
}

void StructuredTopology::setShape( const xdm::DataShape<>& shape ) {
  mShape = shape;
}

const xdm::DataShape<>& StructuredTopology::shape() const {
  return mShape;
}

void StructuredTopology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Topology::writeMetadata( xml );

  // write the shape of the topology
  std::stringstream ss;
  ss << mShape;
  xml.setAttribute( "Dimensions", ss.str() );
}

XDM_GRID_NAMESPACE_END

