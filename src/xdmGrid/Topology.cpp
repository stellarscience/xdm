//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmGrid/Topology.hpp>

#include <algorithm>

XDM_GRID_NAMESPACE_BEGIN

Topology::Topology() :
  xdm::Item() {
}

Topology::~Topology() {
}

void Topology::traverse( xdm::ItemVisitor& iv ) {
  std::for_each( begin(), end(), xdm::ApplyVisitor( iv ) );
}

void Topology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::Item::writeMetadata( xml );
  xml.setTag( "Topology" );
}

XDM_GRID_NAMESPACE_END
