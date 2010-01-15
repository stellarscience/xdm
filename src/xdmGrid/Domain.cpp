//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmGrid/Domain.hpp>

#include <algorithm>

XDM_GRID_NAMESPACE_BEGIN

void Domain::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  xdm::Item::writeMetadata( xml );
  xml.setTag( "Domain" );
}

void Domain::traverse( xdm::ItemVisitor& iv ) {
  Item::traverse( iv );
  std::for_each(
    xdm::begin< xdm::DataItem >( *this ),
    xdm::end< xdm::DataItem >( *this ),
    xdm::ApplyVisitor( iv ) );
  std::for_each( 
    xdm::begin< Grid >( *this ),
    xdm::end< Grid >( *this ),
    xdm::ApplyVisitor( iv ) );
}

XDM_GRID_NAMESPACE_END

