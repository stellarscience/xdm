#include <xdm/CompositeDataItem.hpp>

#include <algorithm>

#include <cassert>

XDM_NAMESPACE_BEGIN

CompositeDataItem::CompositeDataItem() :
  mChildData() {
}

CompositeDataItem::CompositeDataItem( unsigned int n ) :
  mChildData( n ) {
}

CompositeDataItem::~CompositeDataItem() {
}

void CompositeDataItem::setNumberOfChildren( unsigned int n ) {
  mChildData.resize( n );
}

unsigned int CompositeDataItem::numberOfChildren() const {
  return mChildData.size();
}

void CompositeDataItem::appendChild( DataItem* child ) {
  mChildData.push_back( child );
}

void CompositeDataItem::setChild( unsigned int i, RefPtr< DataItem > data ) {
  assert( i < mChildData.size() );
  mChildData[i] = data;
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

void CompositeDataItem::writeMetadata( XmlMetadataWrapper& xml ) {
  DataItem::writeMetadata( xml );

  // write my ItemType as a collection.
  xml.setAttribute( "ItemType", "Tree" );
}

XDM_NAMESPACE_END

