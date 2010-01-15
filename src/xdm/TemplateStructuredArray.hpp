//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdm_TemplateStructuredArray_hpp
#define xdm_TemplateStructuredArray_hpp

#include <xdm/PrimitiveType.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/StructuredArray.hpp>

#include <cassert>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Attaches type information to a StructuredArray and provides convenience
/// functions for accessing typed data stored in the array.  Access to the
/// underlying data is exposed as an STL compliant random access iterator.
template< typename T >
class TemplateStructuredArray : public StructuredArray {
public:
  //-- iterators to expose the underlying data --//
  typedef T value_type;
  typedef T* iterator;
  typedef const T* const_iterator;
 
  /// Constructor takes a pointer to the data for this type and a shape for the
  /// array.
  TemplateStructuredArray( T* data, const DataShape<>& shape ) :
    StructuredArray( 
      static_cast< primitiveType::Value >( PrimitiveTypeInfo< T >::kValue ),
      data,
      shape ) {}

  virtual ~TemplateStructuredArray() {}

  /// Get an iterator pointing to the beginning of the data.
  iterator begin() {
    return static_cast< T* >( data() );
  }

  /// Get a const iterator pointing to the beginning of the data.
  const_iterator begin() const {
    return static_cast< const T* >( data() );
  }
  
  /// Get an iterator pointing to the end of the data.
  iterator end() {
    return static_cast< T* >( data() ) + size();
  }

  /// Get a const iterator pointing to the end of the data.
  const_iterator end() const {
    return static_cast< const T* >( data() ) + size();
  }

  /// Index the ith element of the array.
  value_type& operator[]( size_t i ) {
    assert( i < size() );
    return *(static_cast< T* >( data() ) + i);
  }

  /// Index the const ith element of the array.
  const value_type& operator[] ( size_t i ) const {
    assert( i < size() );
    return *(static_cast< const T* >( data() ) + i);
  }
};

/// Convenience template to construct a StructuredArray with the right type
/// arguments. This function uses template deduction to determine the right type
/// of TemplateStructuredArray to instantiate.
template< typename T >
xdm::RefPtr< StructuredArray >
createStructuredArray( T* data, const DataShape<>& shape ) {
  xdm::RefPtr< StructuredArray > result(
    new TemplateStructuredArray< T >( data, shape ) );
  return result;
}

XDM_NAMESPACE_END

#endif // xdm_TemplateStructuredArray_hpp
