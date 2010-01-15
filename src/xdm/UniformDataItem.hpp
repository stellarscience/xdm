#ifndef xdm_UniformDataItem_hpp
#define xdm_UniformDataItem_hpp

#include <xdm/DataSelectionMap.hpp>
#include <xdm/Dataset.hpp>
#include <xdm/HyperSlab.hpp>
#include <xdm/DataItem.hpp>
#include <xdm/StructuredArray.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Terminal node containing the actual heavy data sets.  A UniformDataItem contains
/// all of the information required to map a StructuredArray in memory to a
/// Dataset in a file. 
class UniformDataItem : public DataItem {
public:
  explicit UniformDataItem( const DataShape<>& dataspace = DataShape<>() );
  virtual ~UniformDataItem();

  XDM_META_ITEM( UniformDataItem );

  /// Determine if the DataItem changes over time.
  bool isDynamic() const { return mIsDynamic; }
  /// Specify whether or not a DataItem changes over time.
  void setIsDynamic( bool isDynamic ) { mIsDynamic = isDynamic; }

  StructuredArray* array();
  const StructuredArray* array() const;
  void setArray( StructuredArray* array );

  Dataset* dataset();
  const Dataset* dataset() const;
  void setDataset( Dataset* ds );

  DataShape<>& dataspace();
  const DataShape<>& dataspace() const;
  void setDataspace( const DataShape<>& dataspace );

  const DataSelectionMap& selectionMap() const;
  void setSelectionMap( const DataSelectionMap& selectionMap );

  virtual void writeMetadata( XmlMetadataWrapper& xml );

  /// Virtual function to serialize the data contained in the item.  Inheritors
  /// should reimplement this function to specify what data should be written to
  /// a dataset.
  virtual void serializeData();

private:
  bool mIsDynamic;
  DataShape<> mDataspace;
  RefPtr< StructuredArray > mArray;
  RefPtr< Dataset > mDataset;
  DataSelectionMap mSelectionMap;
};

XDM_NAMESPACE_END

#endif // xdm_UniformDataItem_hpp

