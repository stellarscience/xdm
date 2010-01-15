#ifndef xdmGrid_NamespaceMacro_hpp
#define xdmGrid_NamespaceMacro_hpp

/// Components for declaring grid objects. This library contains objects that
/// define structured and unstructured grids and provides the capacity to define
/// attributes over the grids. The grid implementations as well as the
/// implementations for different geometry definitions and multiple structured
/// and unstructured topology definitions are defined here.
namespace xdmGrid { }

#ifndef SWIG
# define XDM_GRID_NAMESPACE_BEGIN namespace xdmGrid {
#else
# define XDM_GRID_NAMESPACE_BEGIN
#endif

#ifndef SWIG
# define XDM_GRID_NAMESPACE_END }
#else
# define XDM_GRID_NAMESPACE_END
#endif

#endif // xdmGrid_NamespaceMacro_hpp

