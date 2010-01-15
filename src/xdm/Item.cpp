#include <xdm/Item.hpp>

#include <xdm/ItemVisitor.hpp>

XDM_NAMESPACE_BEGIN

Item::Item() {
}

Item::~Item() {
}

void Item::accept( ItemVisitor& ) {
  //no op
}

void Item::traverse( ItemVisitor& ) {
  //no op
}

XDM_NAMESPACE_END

