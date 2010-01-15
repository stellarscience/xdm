#include <xdm/SerializeDataOperation.hpp>

#include <xdm/UniformDataItem.hpp>

XDM_NAMESPACE_BEGIN

SerializeDataOperation::SerializeDataOperation() {
}

SerializeDataOperation::~SerializeDataOperation() {
}

void SerializeDataOperation::apply( UniformDataItem& udi ) {
  udi.serializeData();
}

XDM_NAMESPACE_END

