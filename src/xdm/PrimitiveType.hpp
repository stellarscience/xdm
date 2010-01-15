#ifndef xdm_Primitive_hpp
#define xdm_Primitive_hpp

#include <complex>
#include <string>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

typedef char int8;
typedef short int16;
typedef int int32;
typedef long int int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef long unsigned int uint64;
typedef float float32;
typedef double float64;
typedef std::complex< float32, float32 > complex_float32;
typedef std::complex< float64, float64 > complex_float64;
typedef std::string string;

XDM_NAMESPACE_END

#endif // xdm_Primitive_hpp

