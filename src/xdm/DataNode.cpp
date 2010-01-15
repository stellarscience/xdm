#include <xdm/DataNode.hpp>

XDM_NAMESPACE_BEGIN

DataNode::DataNode( const DataShape<>& dataspace ) :
  mDataspace( dataspace ),
  mArray(),
  mDataset() {
}

DataNode::~DataNode() {
}

StructuredArray* DataNode::array() {
  return mArray.get();
}

const StructuredArray* DataNode::array() const {
  return mArray.get();
}

void DataNode::setArray( StructuredArray* array ) {
  mArray = array;
}

Dataset* DataNode::dataset() {
  return mDataset.get();
}

const Dataset* DataNode::dataset() const {  
  return mDataset.get();
}

void DataNode::setDataset( Dataset* ds ) {
  mDataset = ds;
}

DataShape<>& DataNode::dataspace() {
  return mDataspace;
}

const DataShape<>& DataNode::dataspace() const {
  return mDataspace;
}

void DataNode::setDataspace( const DataShape<>& dataspace ) {
  mDataspace = dataspace;
}

XDM_NAMESPACE_END

