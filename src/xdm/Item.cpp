#include <xdm/Item.hpp>

#include <xdm/ItemVisitor.hpp>

XDM_NAMESPACE_BEGIN

Item::Item() {
}

Item::~Item() {
}

void Item::setName( const std::string& name ) {
  mName = name;
}

const std::string& Item::name() const {
  return mName;
}

const char * Item::className() const {
  return "Item";
}

void Item::accept( ItemVisitor& iv ) {
  iv.apply( *this );
}

void Item::traverse( ItemVisitor& ) {
  // No-op
}

void Item::writeMetadata( XmlMetadataWrapper& xml ) {
  xml.setTag( className() );
  if ( !mName.empty() ) {
    xml.setAttribute( "Name", mName );
  }
}

XDM_NAMESPACE_END

