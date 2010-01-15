#include <xdm/DataSelection.hpp>

#include <xdm/DataSelectionVisitor.hpp>

XDM_NAMESPACE_BEGIN

DataSelection::DataSelection() {
}

DataSelection::~DataSelection() {
}

void DataSelection::accept( DataSelectionVisitor& v ) {
  v.apply( *this );
}

XDM_NAMESPACE_END

