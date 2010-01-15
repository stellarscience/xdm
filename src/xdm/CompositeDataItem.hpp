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
  
  /// Constructor takes the number of children to hold.
  explicit CompositeDataItem( unsigned int n );

  virtual ~CompositeDataItem();

  XDM_META_ITEM( CompositeDataItem );

  /// Set the number of children contained by this DataItem.
  void setNumberOfChildren( unsigned int n );
  /// Get the number of children contained by this DataItem.
  unsigned int numberOfChildren() const;

  /// Add a child Data element to the end of the list.  This will increase the
  /// size of the list of children.
  virtual void appendChild( DataItem* child );
  /// Set the i_th child of the data item.
  virtual void setChild( unsigned int i, RefPtr< DataItem > data );
  
  /// Get the const child item at the numbered location.
  const DataItem* child( unsigned int i ) const;
  /// Get the child item at the numbered location.
  DataItem* child( unsigned int i );

  /// Redefinition of visitor traversal from Item.
  virtual void traverse( ItemVisitor& iv );

  /// Write the metadata for a collection of dataitems.
  virtual void writeMetadata( XmlMetadataWrapper& xml );
private:
  std::vector< RefPtr< DataItem > > mChildData;
};

XDM_NAMESPACE_END

#endif // xdm_CompositeDataItem_hpp

