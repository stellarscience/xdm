#include <xdm/DataSelectionVisitor.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/CoordinateDataSelection.hpp>
#include <xdm/HyperslabDataSelection.hpp>

XDM_NAMESPACE_BEGIN

DataSelectionVisitor::DataSelectionVisitor() {
}

DataSelectionVisitor::~DataSelectionVisitor() {
}

void DataSelectionVisitor::apply( const DataSelection& selection ) {
  // no-op
}

void DataSelectionVisitor::apply( const AllDataSelection& selection ) {
  apply( static_cast< const DataSelection& >( selection ) );
}

void DataSelectionVisitor::apply( const HyperslabDataSelection& selection ) {
  apply( static_cast< const DataSelection& >( selection ) );
}

void DataSelectionVisitor::apply( const CoordinateDataSelection& selection ) {
  apply( static_cast< const DataSelection& >( selection ) );
}

XDM_NAMESPACE_END

