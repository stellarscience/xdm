#ifndef xdm_StructuredArray_hpp
#define xdm_StructuredArray_hpp

#include <xdm/DataShape.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/SelectableDataMixin.hpp>

#include <xdm/NamespaceMacro.hpp>

#include <vector>

XDM_NAMESPACE_BEGIN

/// Array class that holds a multi-dimensional structured array.  The data has
/// an associated shape and selection.  This class does not own the data, it
/// merely holds a pointer to it.  This is to allow client applications to
/// choose their own memory arrangements.
class StructuredArray : public ReferencedObject {
public:
  /// Construct an array given a primitive type, a pointer to the data, and a
  /// shape.  Generally clients will want to use TemplateStructuredArray to
  /// ensure this class is constructed with the correct parameters.
  ///
  /// @param type Enumerated value for the type of array.
  /// @param arrayElementSize The size of a single array element in bytes.
  /// @param data A pointer to the actual data in memory.
  /// @param shape The shape of the data in memory.
  StructuredArray(
    const primitiveType::Value& type,
    size_t arrayElementSize,
    void* data,
    const DataShape<>& shape );
  
  virtual ~StructuredArray();

  /// Get the type of the data.
  primitiveType::Value dataType() const {
    return mType;
  }

  /// Get the size of the array elements in bytes.
  size_t elementSize() const;

  /// Get the number of data elements.
  size_t size() const;

  /// Get a pointer to the actual data.
  void* data();
  /// Get a const pointer to the actual data.
  const void* data() const;

  /// Get the shape of the data.
  const DataShape<>& shape() const {  
    return mShape;
  }

  /// Set the shape of the data.
  void setShape( const DataShape<>& shape ) {
    mShape = shape;
  }

protected:
  /// Allow subclasses to set the data pointer, but no one else.
  void setData( void* ptr ) {
    mData = ptr;
  }

private:
  primitiveType::Value mType;
  size_t mArrayElementSize;
  void* mData;
  DataShape<> mShape;
};

XDM_NAMESPACE_END

#endif // xdm_StructuredArray_hpp

