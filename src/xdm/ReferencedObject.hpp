#ifndef xdm_ReferencedObject_hpp
#define xdm_ReferencedObject_hpp

#include <cstdlib>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Base class for all reference counted objects.
class ReferencedObject {
public:
  ReferencedObject();
  virtual ~ReferencedObject();

  /// Add a reference to this object.
  void addReference();

  /// Remove a reference from this object.  When the reference count goes to
  /// zero, the object will be deleted.
  void removeReference();
  /// Remove a reference without checking the reference count.
  void removeReferenceWithoutDelete();

private:
  std::size_t mReferenceCount;
};

XDM_NAMESPACE_END

#endif // xdm_ReferencedObject_hpp

