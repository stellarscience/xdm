#include <xdm/DataItem.hpp>

XDM_NAMESPACE_BEGIN

DataItem::DataItem() {
}

DataItem::~DataItem() {
}

void DataItem::writeMetadata( XmlMetadataWrapper& xml ) {
  Item::writeMetadata( xml );
  xml.setTag( "DataItem" );
}

XDM_NAMESPACE_END

