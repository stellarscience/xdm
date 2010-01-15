#include <xdm/Node.hpp>

XDM_NAMESPACE_BEGIN

Node::Node() {
}

Node::~Node() {
}

void Node::setName( const std::string& name ) {
  mName = name;
}

std::string& Node::name() const {
  return mName;
}

XDM_NAMESPACE_END

