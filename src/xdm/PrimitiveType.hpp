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

/// Structure to provide static type information for values of the
/// primitiveType::Value enumeration.
template< typename T > struct PrimitiveTypeInfo {
  /// A compile error on this line means a specialization of PrimitiveTypeInfo
  /// has not been defined for the type T.  A specialization must be defined
  /// that provides values for PrimitiveTypeInfo::kSize and
  /// PrimitiveTypeInfo::kValue.
  typedef typename T::NoSpecializationForRequestedType StaticFailure;
};

#define XDM_PRIMITIVE_TYPE_DEFINE_INFO( type, value ) \
  template<> struct PrimitiveTypeInfo< type > { \
    enum { kValue = value }; \
    enum { kSize = sizeof( type ) }; \
  };

XDM_PRIMITIVE_TYPE_DEFINE_INFO( char, primitiveType::kChar )
XDM_PRIMITIVE_TYPE_DEFINE_INFO( short, primitiveType::kShort )
XDM_PRIMITIVE_TYPE_DEFINE_INFO( int, primitiveType::kInt )
XDM_PRIMITIVE_TYPE_DEFINE_INFO( long int, primitiveType::kLongInt )
XDM_PRIMITIVE_TYPE_DEFINE_INFO( unsigned char, primitiveType::kUnsignedChar )
XDM_PRIMITIVE_TYPE_DEFINE_INFO( unsigned short, primitiveType::kUnsignedShort )
XDM_PRIMITIVE_TYPE_DEFINE_INFO( unsigned int, primitiveType::kUnsignedInt )
XDM_PRIMITIVE_TYPE_DEFINE_INFO( long unsigned int, primitiveType::kLongUnsignedInt )
XDM_PRIMITIVE_TYPE_DEFINE_INFO( float, primitiveType::kFloat )
XDM_PRIMITIVE_TYPE_DEFINE_INFO( double, primitiveType::kDouble )

#undef XDM_PRIMITIVE_TYPE_DEFINE_INFO

XDM_NAMESPACE_END

#endif // xdm_PrimitiveType_hpp

