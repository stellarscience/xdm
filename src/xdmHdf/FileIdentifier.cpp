//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#include <xdmHdf/FileIdentifier.hpp>
#include <xdmHdf/FileIdentifierRegistry.hpp>

#include <hdf5.h>

XDM_HDF_NAMESPACE_BEGIN

xdm::RefPtr< FileIdentifier >
createFileIdentifier( const std::string& filename ) {
  return FileIdentifierRegistry::instance()->findOrCreateIdentifier( filename );
}

XDM_HDF_NAMESPACE_END

