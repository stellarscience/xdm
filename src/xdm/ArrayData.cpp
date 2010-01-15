#include <xdm/ArrayData.hpp>

XDM_NAMESPACE_BEGIN

ArrayData::ArrayData( const DataShape<>& dataspace ) :
  mDataspace( dataspace ),
  mArray(),
  mDataset() {
}

ArrayData::~ArrayData() {
}

StructuredArray* ArrayData::array() {
  return mArray.get();
}

const StructuredArray* ArrayData::array() const {
  return mArray.get();
}

void ArrayData::setArray( StructuredArray* array ) {
  mArray = array;
}

Dataset* ArrayData::dataset() {
  return mDataset.get();
}

const Dataset* ArrayData::dataset() const {  
  return mDataset.get();
}

void ArrayData::setDataset( Dataset* ds ) {
  mDataset = ds;
}

DataShape<>& ArrayData::dataspace() {
  return mDataspace;
}

const DataShape<>& ArrayData::dataspace() const {
  return mDataspace;
}

void ArrayData::setDataspace( const DataShape<>& dataspace ) {
  mDataspace = dataspace;
}

XDM_NAMESPACE_END

