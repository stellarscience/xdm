#include <xdm/SerializeDataOperation.hpp>

#include <xdm/UniformDataItem.hpp>

XDM_NAMESPACE_BEGIN

SerializeDataOperation::SerializeDataOperation() {
}

SerializeDataOperation::~SerializeDataOperation() {
}

void SerializeDataOperation::apply( UniformDataItem& udi ) {
  Dataset* dataset = udi.dataset();
  StructuredArray* array = udi.array();
  dataset->initialize( array->dataType(), udi.dataspace() );
  dataset->serialize( array, udi.selectionMap() );
  dataset->finalize();
}

XDM_NAMESPACE_END

