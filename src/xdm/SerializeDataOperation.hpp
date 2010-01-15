#ifndef xdm_SerializeDataOperation_hpp
#define xdm_SerializeDataOperation_hpp

#include <xdm/ItemVisitor.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

class UniformDataItem;

/// Operation defined on the data tree that serializes all datasets it
/// encounters.  Implements the ItemVisitor interface to find all
/// UniformDataItems in the tree and serialize their heavy datasets.
class SerializeDataOperation : public ItemVisitor {
public:
  SerializeDataOperation();
  virtual ~SerializeDataOperation();

  /// Serialize a UniformDataItem's array into its dataset.
  virtual void apply( UniformDataItem& udi );
};

XDM_NAMESPACE_END

#endif // xdm_SerializeDataOperation_hpp

