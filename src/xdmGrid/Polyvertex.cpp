//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmGrid/Polyvertex.hpp>

XDM_GRID_NAMESPACE_BEGIN

Polyvertex::Polyvertex() :
  UnstructuredTopology() {
}

Polyvertex::~Polyvertex() {
}

void Polyvertex::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  UnstructuredTopology::writeMetadata( xml );

  xml.setAttribute( "TopologyType", "Polyvertex" );
}

XDM_GRID_NAMESPACE_END
