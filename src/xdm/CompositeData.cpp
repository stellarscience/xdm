#include <xdm/CompositeData.hpp>

#include <cassert>

XDM_NAMESPACE_BEGIN

CompositeData::CompositeData() :
  mChildData() {
}

CompositeData::~CompositeData() {
}

void CompositeData::appendChild( DataItem* child ) {
  mChildData.push_back( child );
}

const DataItem* CompositeData::child( unsigned int i ) const {
  assert( i < mChildData.size() );
  return mChildData[i];
}

DataItem* CompositeData::child( unsigned int i ) {
  // call the const version to minimize code duplication.
  return const_cast< DataItem* >( 
    (static_cast< const CompositeData* >(this))->child(i));
}

XDM_NAMESPACE_END

