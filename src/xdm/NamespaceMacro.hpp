#ifndef xdm_NamespaceMacro_hpp
#define xdm_NamespaceMacro_hpp

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

