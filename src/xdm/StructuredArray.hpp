#ifndef xdm_StructuredArray_hpp
#define xdm_StructuredArray_hpp

#include <xdm/DataShape.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

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
  /// shape.
  StructuredArray(
    const primitiveType::Value& type,
    void* data,
    const DataShape<>& shape );
  
  virtual ~StructuredArray();

  /// Get the type of the data.
  primitiveType::Value dataType() const {
    return mType;
  }

  /// Get the number of data elements.
  size_t dataSize() const;

  /// Get a pointer to the actual data.
  void* data();
  /// Get a const pointer to the actual data.
  const void* data() const;

  /// Get the shape of the data.
  const DataShape<>& shape() const {  
    return mShape;
  }

private:
  primitiveType::Value mType;
  void* mData;
  DataShape<> mShape;
};

XDM_NAMESPACE_END

#endif // xdm_StructuredArray_hpp

