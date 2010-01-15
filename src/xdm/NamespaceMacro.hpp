//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_NamespaceMacro_hpp
#define xdm_NamespaceMacro_hpp

/// Core object model and data interfaces. The xdm namespace contains the
/// definitions of the core xdm data model and interfaces for the data
/// abstraction layer. All other components may use xdm.
namespace xdm {

  /// Implementation details for xdm.
  namespace detail { }

  /// Typedefs for declaring primitive objects in xdm.
  namespace primitiveType { }
}

#ifndef SWIG
# define XDM_NAMESPACE_BEGIN namespace xdm {
#else
# define XDM_NAMESPACE_BEGIN
#endif

#ifndef SWIG
# define XDM_NAMESPACE_END }
#else
# define XDM_NAMESPACE_END
#endif

#endif // xdm_NamespaceMacro_hpp

