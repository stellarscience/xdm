#include <xdm/UniformDataItem.hpp>

#include <xdm/DataSelection.hpp>

#include <sstream>
#include <stdexcept>

#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

UniformDataItem::UniformDataItem( const DataShape<>& dataspace ) :
  mIsDynamic( false ),
  mDataspace( dataspace ),
  mArray(),
  mDataset(),
  mSelectionMap() {
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

void UniformDataItem::writeMetadata( XmlMetadataWrapper& xml ) {
  DataItem::writeMetadata( xml );

  // write the data item type to "Uniform"
  xml.setAttribute( "ItemType", "Uniform" );
  
  // write the dimensions of the data on disk
  std::stringstream dimensions;
  dimensions << mDataspace;
  xml.setAttribute( "Dimensions", dimensions.str() );

  // write the type of data to the xml metadata.
  primitiveType::Value type = mArray->dataType();
  switch ( type ) {
  case primitiveType::kChar:
    xml.setAttribute( "NumberType", "Char" );
    break;
  case primitiveType::kUnsignedChar:
    xml.setAttribute( "NumberType", "UChar" );
    break;
  case primitiveType::kInt:
    xml.setAttribute( "NumberType", "Int" );
    break;
  case primitiveType::kUnsignedInt:
    xml.setAttribute( "NumberType", "UInt" );
    break;
  case primitiveType::kFloat:
  case primitiveType::kDouble:
    xml.setAttribute( "NumberType", "Float" );
    break;
  default:
    XDM_THROW( std::invalid_argument( "Unrecognized dataset number type" ) );
    break;
  }

  // write the size of the data to the metadata.
  xml.setAttribute( "Precision", mArray->elementSize() );

  // write the format of the dataset on disk.
  xml.setAttribute( "Format", mDataset->format() );

  // Allow the dataset to configure the text content for this item.
  mDataset->writeTextContent( xml );
}

const DataSelectionMap& UniformDataItem::selectionMap() const {
  return mSelectionMap;
}

void UniformDataItem::setSelectionMap( const DataSelectionMap& selectionMap ) {
  mSelectionMap = selectionMap;
}

void UniformDataItem::serializeData() {
  mDataset->initialize( mArray->dataType(), mDataspace );
  mDataset->serialize( mArray, mSelectionMap );
  mDataset->finalize();
}

XDM_NAMESPACE_END

