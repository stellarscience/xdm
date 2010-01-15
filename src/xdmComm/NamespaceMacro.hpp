#ifndef xdmComm_NamespaceMacro_hpp
#define xdmComm_NamespaceMacro_hpp

/// Component library that defines a communication strategy for writing data to
/// disk. This library contains implementations and utilities for passing data
/// between processes in an parallel, distributed memory environment.
namespace xdmComm { }

#ifndef SWIG
# define XDM_COMM_NAMESPACE_BEGIN namespace xdmComm {
#else
# define XDM_COMM_NAMESPACE_BEGIN
#endif

#ifndef SWIG
# define XDM_COMM_NAMESPACE_END }
#else
# define XDM_COMM_NAMESPACE_END
#endif

#endif // xdmComm_NamespaceMacro_hpp

