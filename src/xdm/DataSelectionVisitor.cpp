#include <xdm/DataSelectionVisitor.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/HyperslabDataSelection.hpp>

XDM_NAMESPACE_BEGIN

DataSelectionVisitor::DataSelectionVisitor() {
}

DataSelectionVisitor::~DataSelectionVisitor() {
}

void DataSelectionVisitor::apply( DataSelection& selection ) {
  // no-op
}

void DataSelectionVisitor::apply( AllDataSelection& selection ) {
  apply( static_cast< DataSelection& >( selection ) );
}

void DataSelectionVisitor::apply( HyperslabDataSelection& selection ) {
  apply( static_cast< DataSelection& >( selection ) );
}

XDM_NAMESPACE_END

