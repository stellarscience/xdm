#ifndef xdm_PrimitiveType_hpp
#define xdm_PrimitiveType_hpp

#include <complex>
#include <string>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

namespace primitiveType {
  /// Enumeration of type names.
  enum Value {
    kChar = 0,
    kShort,
    kInt,
    kLongInt,
    kUnsignedChar,
    kUnsignedShort,
    kUnsignedInt,
    kLongUnsignedInt,
    kFloat,
    kDouble  
  };
} // namespace primitiveType

XDM_NAMESPACE_END

#endif // xdm_PrimitiveType_hpp

