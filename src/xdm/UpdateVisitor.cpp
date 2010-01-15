#include <xdm/UpdateVisitor.hpp>

#include <xdm/Dataset.hpp>
#include <xdm/UniformDataItem.hpp>

XDM_NAMESPACE_BEGIN

void UpdateVisitor::apply( UniformDataItem& item ) {
  xdm::Dataset* itemDataset = item.dataset();
  if ( itemDataset ) {
    itemDataset->update();
  }
}

XDM_NAMESPACE_END

