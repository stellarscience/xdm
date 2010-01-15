#ifndef xdmHdf_NamespaceMacro_hpp
#define xdmHdf_NamespaceMacro_hpp

/// Implementations for writing array based datasets using the standard HDF5
/// binary format.  This library contains utilities and implementations of the
/// xdm::Dataset interface for writing data to disk using HDF5 using both the
/// serial and parallel APIs.
namespace xdmHdf { }

#ifndef SWIG
# define XDM_HDF_NAMESPACE_BEGIN namespace xdmHdf {
#else
# define XDM_HDF_NAMESPACE_BEGIN
#endif

#ifndef SWIG
# define XDM_HDF_NAMESPACE_END }
#else
# define XDM_HDF_NAMESPACE_END
#endif

#endif // xdmHdf_NamespaceMacro_hpp

