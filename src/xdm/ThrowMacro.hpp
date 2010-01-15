//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_ThrowMacro_hpp
#define xdm_ThrowMacro_hpp

#include <sstream>

/// Macro to wrap exceptions to add source code location content and disable
/// them if necessary.
#define XDM_THROW( exception ) \
  throw exception

#endif // xdm_ThrowMacro_hpp

