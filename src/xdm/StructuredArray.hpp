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

/// Array class for holding multiple types of data. This class represents a
/// contiguous array of data in memory.  It does not own the data it holds, it
/// merely presents an interface for querying information about an array that is
/// useful when reading or writing data.  The TemplateStructuredArray subclass
/// implements a type safe interface for providing this information.
///
/// This class does not own the data it references, it is up to subclasses to
/// to manage (or not manage) memory. This is to allow applications to wrap
/// a StructuredArray around existing memory structures.
///
/// @see TemplateStructuredArray
/// @see VectorStructuredArray
class StructuredArray : public ReferencedObject {
public:
  
  StructuredArray();
  virtual ~StructuredArray();

  /// Get the type of the data.
  virtual primitiveType::Value dataType() const = 0;

  /// Get the size of the array elements in bytes.
  virtual size_t elementSize() const = 0;

  /// Get the number of data elements.
  virtual size_t size() const = 0;

  /// Get a const pointer to the actual data.
  virtual const void* data() const = 0;

  /// Get a pointer to the array data. The default implementation uses the
  /// const qualified version, although it is virtual in case subclasses need
  /// another implementation.
  ///
  /// The non-const version calling the const version is an idiom described in
  /// Myers' Effective C++ Third Edition p. 24. It is very important that the
  /// non-const version call the const version and not the other way around.
  virtual void* data() {
    return const_cast< void* >( static_cast< StructuredArray& >(*this).data());
  }

  /// Get the size of the array in memory in bytes.
  size_t memorySize() const;
};

XDM_NAMESPACE_END

#endif // xdm_StructuredArray_hpp

