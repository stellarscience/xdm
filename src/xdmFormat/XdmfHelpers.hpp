//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmFormat_XdmfHelpers_hpp
#define xdmFormat_XdmfHelpers_hpp

#include <xdm/RefPtr.hpp>

#include <string>

#include <xdmFormat/NamespaceMacro.hpp>
#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN
  class XmlObject;
XDM_NAMESPACE_END

XDM_FORMAT_NAMESPACE_BEGIN

/// Create the root XDMF xml object that must be the root of any XDMF tree.
xdm::RefPtr< xdm::XmlObject > createXdmfRoot( 
  const std::string& version = "2.1" );

XDM_FORMAT_NAMESPACE_END

#endif // xdmFormat_XdmfHelpers_hpp

