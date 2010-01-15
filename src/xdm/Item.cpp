#include <xdm/Item.hpp>

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

XDM_NAMESPACE_END

