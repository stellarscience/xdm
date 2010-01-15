#ifndef xdm_CompositeDataItem_hpp
#define xdm_CompositeDataItem_hpp

#include <xdm/DataItem.hpp>
#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Class that groups multiple Data nodes together.  CompositeDataItem is a Data
/// type that can hold child Data types.  It is used to construct a large
/// dataset from multiple smaller datasets.
class CompositeDataItem : public DataItem {
public:
  CompositeDataItem();
  virtual ~CompositeDataItem();

  XDM_META_ITEM( CompositeDataItem );

  /// Add a child Data element.
  virtual void appendChild( DataItem* child );
  
  /// Get the const child item at the numbered location.
  const DataItem* child( unsigned int i ) const;
  /// Get the child item at the numbered location.
  DataItem* child( unsigned int i );

  /// Redefinition of visitor traversal from Item.
  virtual void traverse( ItemVisitor& iv );
private:
  std::vector< RefPtr< DataItem > > mChildData;
};

XDM_NAMESPACE_END

#endif // xdm_CompositeDataItem_hpp

