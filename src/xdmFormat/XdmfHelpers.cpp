//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmFormat/XdmfHelpers.hpp>

#include <xdm/XmlObject.hpp>

XDM_FORMAT_NAMESPACE_BEGIN

xdm::RefPtr< xdm::XmlObject > createXdmfRoot( const std::string& version ) {
  xdm::RefPtr< xdm::XmlObject > xdmf( new xdm::XmlObject( "Xdmf" ) );
  xdmf->appendAttribute( "Version", version );
  return xdmf;
}

XDM_FORMAT_NAMESPACE_END

