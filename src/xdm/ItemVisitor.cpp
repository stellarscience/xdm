#include <xdm/ItemVisitor.hpp>

#include <xdm/Item.hpp>

XDM_NAMESPACE_BEGIN

ItemVisitor::ItemVisitor() {
}

ItemVisitor::~ItemVisitor() {
}

void ItemVisitor::traverse( Item& item ) {
  item.traverse( *this );
}

void ItemVisitor::apply( Item& item ) {
  traverse( item );
}

void applyVisitor( ItemVisitor& iv, Item& item ) {
  item.accept( iv );
}

XDM_NAMESPACE_END

