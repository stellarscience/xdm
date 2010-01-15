#ifndef xdm_TemplateStructuredArray_hpp
#define xdm_TemplateStructuredArray_hpp

#include <xdm/PrimitiveType.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/StructuredArray.hpp>

#include <cassert>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Presents a contiguous c-style array as a StructuredArray for use in the xdm
/// interfaces. This class does not take ownership of the array.  This is to
/// allow clients to pass their existing data structure to the xdm interfaces
/// for I/O purposes.
///
/// In addition to providing the StructuredArray interface for data, this class
/// also provides STL compliant random access iterators for accessing the data
/// as well as indexing operators for array access.
template< typename T >
class TemplateStructuredArray : public StructuredArray {
public:

  //-- STL Container Interface Types --//

  typedef T value_type;
  typedef T* pointer;
  typedef T* iterator; ///< Random access iterator
  typedef const T* const_iterator; ///< Random access const iterator

  /// Construct the array without initializing the data or size. The data
  /// pointer will be NULL and the size will be 0.
  TemplateStructuredArray() :
    StructuredArray(),
    mData( 0 ),
    mSize( 0 ) {
  }
 
  /// Construct the array with a pointer to its data and size.
  TemplateStructuredArray( T* data, size_t size ) :
    StructuredArray(),
    mData( data ),
    mSize( size ) {
  }

  virtual ~TemplateStructuredArray() {}

  //-- StructuredArray Query Interface --//

  virtual primitiveType::Value dataType() const {
    return PrimitiveTypeInfo< T >::kValue;
  }

  virtual size_t elementSize() const {
    return PrimitiveTypeInfo< T >::kSize;
  }

  virtual size_t size() const {
    return mSize;
  }

  virtual const void* data() const {
    return mData;
  }

  //-- Data Access Interface --//

  /// Set the pointer to the array data.
  void setData( T* data ) {
    mData = data;
  }

  /// Set the number of elements in the array.
  void setSize( size_t size ) {
    mSize = size;
  }

  /// Get an iterator pointing to the beginning of the data.
  iterator begin() {
    return mData;
  }

  /// Get a const iterator pointing to the beginning of the data.
  const_iterator begin() const {
    return mData;
  }
  
  /// Get an iterator pointing to the end of the data.
  iterator end() {
    return mData + mSize;
  }

  /// Get a const iterator pointing to the end of the data.
  const_iterator end() const {
    return mData + mSize;
  }

  /// Index the ith element of the array.
  value_type& operator[]( size_t i ) {
    assert( i < mSize );
    return mData + i;
  }

  /// Index the const ith element of the array.
  const value_type& operator[] ( size_t i ) const {
    assert( i < mSize );
    return mData + i;
  }

private:
  T* mData;
  size_t mSize;
};

/// Convenience template to construct a StructuredArray with the right type
/// arguments. This function uses template deduction to determine the right type
/// of TemplateStructuredArray to instantiate.
template< typename T >
RefPtr< StructuredArray >
createStructuredArray( T* data, size_t size ) {
  return new TemplateStructuredArray< T >( data, size );
}

XDM_NAMESPACE_END

#endif // xdm_TemplateStructuredArray_hpp
