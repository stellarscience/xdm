#ifndef xdm_PrimitiveType_hpp
#define xdm_PrimitiveType_hpp

#include <complex>
#include <string>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

typedef char Int8;
typedef short Int16;
typedef int Int32;
typedef long int Int64;
typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned int UInt32;
typedef long unsigned int UInt64;
typedef float Float32;
typedef double Float64;
typedef std::complex< Float32 > ComplexFloat32;
typedef std::complex< Float64 > ComplexFloat64;
typedef std::string String;

namespace primitiveType {
  enum Value {
    kInt8 = 0,
    kInt16,
    kInt32,
    kInt64,
    kUInt8,
    kUInt16,
    kUInt32,
    kUInt64,
    kFloat32,
    kFloat64,
    ComplexFloat32,
    ComplexFloat64,
    String
  };
} // namespace primitiveType

XDM_NAMESPACE_END

#endif // xdm_PrimitiveType_hpp

