#include <xdm/SelectableDataMixin.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelection.hpp>

XDM_NAMESPACE_BEGIN

RefPtr< AllDataSelection > SelectableDataMixin::sAllSelection(
  new AllDataSelection );

SelectableDataMixin::SelectableDataMixin() :
  mSelection( sAllSelection ) {
}

SelectableDataMixin::~SelectableDataMixin() {
}

void SelectableDataMixin::setSelection( DataSelection* selection ) {
  mSelection = selection;
}

const DataSelection* SelectableDataMixin::selection() const {
  return mSelection;
}

DataSelection* SelectableDataMixin::selection() {
  return mSelection;
}

void SelectableDataMixin::selectAll() {
  mSelection = sAllSelection;
}

XDM_NAMESPACE_END

