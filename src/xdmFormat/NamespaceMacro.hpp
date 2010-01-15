#ifndef xdmFormat_NamespaceMacro_hpp
#define xdmFormat_NamespaceMacro_hpp

/// Component library that defines how time series data is formatted when
/// written to disk.  This library contains code for writing XDMF standard files
/// to disk.
namespace xdmFormat { }

#ifndef SWIG
# define XDM_FORMAT_NAMESPACE_BEGIN namespace xdmFormat {
#else
# define XDM_FORMAT_NAMESPACE_BEGIN
#endif

#ifndef SWIG
# define XDM_FORMAT_NAMESPACE_END }
#else
# define XDM_FORMAT_NAMESPACE_END
#endif

#endif // xdmFormat_NamespaceMacro_hpp

