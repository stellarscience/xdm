#include <xdm/CompositeDataItem.hpp>

#include <algorithm>

#include <cassert>

XDM_NAMESPACE_BEGIN

CompositeDataItem::CompositeDataItem() :
  mChildData() {
}

CompositeDataItem::~CompositeDataItem() {
}

void CompositeDataItem::appendChild( DataItem* child ) {
  mChildData.push_back( child );
}

const DataItem* CompositeDataItem::child( unsigned int i ) const {
  assert( i < mChildData.size() );
  return mChildData[i];
}

DataItem* CompositeDataItem::child( unsigned int i ) {
  // call the const version to minimize code duplication.
  return const_cast< DataItem* >( 
    (static_cast< const CompositeDataItem* >(this))->child(i));
}

void CompositeDataItem::traverse( ItemVisitor& iv ) {
  std::for_each( mChildData.begin(), mChildData.end(), ApplyVisitor( iv ) );
}

XDM_NAMESPACE_END

