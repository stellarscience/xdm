#ifndef xdm_RaiseMacro_hpp
#define xdm_RaiseMacro_hpp

#include <sstream>

/// Macro to wrap exceptions to add source code location content and disable
/// them if necessary.
#define XDM_RAISE( exception, content ) \
  throw exception( content )

#endif // xdm_ExceptionMacro_hpp

