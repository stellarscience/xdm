#include <xdm/Item.hpp>

#include <xdm/ItemVisitor.hpp>

XDM_NAMESPACE_BEGIN

Item::Item() {
}

Item::~Item() {
}

void Item::accept( ItemVisitor& iv ) {
  iv.apply( *this );
}

void Item::traverse( ItemVisitor& iv ) {
  ItemList::iterator child;
  for ( child = mChildren.begin(); child != mChildren.end(); ++child ) {
    (*child)->accept( iv );
  }
}

unsigned int Item::childCount() const {
  return mChildren.size();
}

XDM_NAMESPACE_END

