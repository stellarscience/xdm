//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmComm_MpiMessageTag_hpp
#define xdmComm_MpiMessageTag_hpp

#include <xdm/NamespaceMacro.hpp>

XDM_COMM_NAMESPACE_BEGIN

/// Class containing an enumeration to guarantee unique message tags within the
/// library.  NOTE: These message tags are guaranteed to be unique *within* the
/// library.  In order to ensure that these message tags remain unique within an
/// application, it is necessary for clients to provide communicators that apply
/// only to the functions within the xdmComm library.
class MpiMessageTag {
public:
  enum Value {
    kWriteData,
    kProcessCompleted
  };
};

XDM_COMM_NAMESPACE_END

#endif // xdmComm_MpiMessageTag_hpp

