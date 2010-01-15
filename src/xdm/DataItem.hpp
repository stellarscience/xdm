#ifndef xdm_DataItem_hpp
#define xdm_DataItem_hpp

#include <xdm/Item.hpp>
#include <xdm/PrimitiveType.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// A DataItem is a class that holds large datasets.
class DataItem : public Item {
public:
  DataItem();
  virtual ~DataItem();
};

XDM_NAMESPACE_END

#endif // xdm_DataItem_hpp

