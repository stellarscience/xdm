#ifndef xdm_TemplateStructuredArray_hpp
#define xdm_TemplateStructuredArray_hpp

#include <xdm/PrimitiveType.hpp>
#include <xdm/StructuredArray.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

template< typename T > class TemplateStructuredArray;
template< typename T > TemplateStructuredArray< T >* 
  createStructuredArray( T*, const DataShape<>& );

/// Attaches type information to a StructuredArray and provides convenience
/// functions for accessing typed data stored in the array.
template< typename T >
class TemplateStructuredArray : public StructuredArray {
public:
  virtual ~TemplateStructuredArray() {}

  /// Get a const pointer to the underlying data as a typed array.
  const T* typedData() const {
    return static_cast< const T* >( data() );
  }
  
  /// Get a pointer to the underlying data as a typed array.
  T* typedData() {
    return static_cast< T* >( data() );
  }

private:
  // provide constructor access to helper templates defined below.
  friend TemplateStructuredArray< T >* createStructuredArray< T >(
    T* data,
    const DataShape<>& shape );
  
  /// Private constructor to ensure the array is constructed with the correct
  /// arguments.
  TemplateStructuredArray(
    primitiveType::Value type,
    T* data,
    const DataShape<>& shape ) :
    StructuredArray( type, data, shape ) {
  }
};

/// Convenience template to construct a StructuredArray with the right type
/// arguments.
/// @see primitiveType::Value
template< typename T >
TemplateStructuredArray< T >* 
createStructuredArray( T* data, const DataShape<>& shape ) {
  // a compile error on the following line means that a createStructuredArray
  // needs a specialization for the type you are trying to pass in as a template
  // parameter.  See the below specializations for an example on how to handle
  // this.
  typedef typename T::no_specialization_for_requested_type fail;
}

/// Helper macro to assist in easily defining specializations of
/// createStructuredArray for a given type and enumerant in 
/// primitiveType::Value.
#define XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( type, value ) \
  template<> \
  TemplateStructuredArray< type >* \
  createStructuredArray< type >( type* data, const DataShape<>& shape ) { \
    return new TemplateStructuredArray< type >( (value), data, shape ); \
  }

XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( char, primitiveType::kChar )
XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( short, primitiveType::kShort )
XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( int, primitiveType::kInt )
XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( long int, primitiveType::kLongInt )
XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( unsigned char, primitiveType::kUnsignedChar )
XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( unsigned short, primitiveType::kUnsignedShort )
XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( unsigned int, primitiveType::kUnsignedInt )
XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( long unsigned int, primitiveType::kLongUnsignedInt )
XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( float, primitiveType::kFloat )
XDM_CREATE_STRUCTURED_ARRAY_DEFINITION( double, primitiveType::kDouble )

XDM_NAMESPACE_END

#endif // xdm_TemplateStructuredArray_hpp
