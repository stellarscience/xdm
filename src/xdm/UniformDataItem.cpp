#include <xdm/UniformDataItem.hpp>

XDM_NAMESPACE_BEGIN

UniformDataItem::UniformDataItem( const DataShape<>& dataspace ) :
  mDataspace( dataspace ),
  mArray(),
  mDataset() {
}

UniformDataItem::~UniformDataItem() {
}

StructuredArray* UniformDataItem::array() {
  return mArray.get();
}

const StructuredArray* UniformDataItem::array() const {
  return mArray.get();
}

void UniformDataItem::setArray( StructuredArray* array ) {
  mArray = array;
}

Dataset* UniformDataItem::dataset() {
  return mDataset.get();
}

const Dataset* UniformDataItem::dataset() const {  
  return mDataset.get();
}

void UniformDataItem::setDataset( Dataset* ds ) {
  mDataset = ds;
}

DataShape<>& UniformDataItem::dataspace() {
  return mDataspace;
}

const DataShape<>& UniformDataItem::dataspace() const {
  return mDataspace;
}

void UniformDataItem::setDataspace( const DataShape<>& dataspace ) {
  mDataspace = dataspace;
}

XDM_NAMESPACE_END

