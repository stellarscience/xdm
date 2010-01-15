#ifndef xdm_Node_hpp
#define xdm_Node_hpp

#include <xdm/ReferencedObject.hpp>

#include <string>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Base class for all elements of the data hierarchy.
class Node : public ReferencedObject {
public:
  Node();
  virtual ~Node();

  void setName( const std::string& name );
  std::string& name() const;

private:
  std::string mName;
};

XDM_NAMESPACE_END

#endif // xdm_Node_hpp

