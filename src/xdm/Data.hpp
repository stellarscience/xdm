#ifndef xdm_Data_hpp
#define xdm_Data_hpp

#include <xdm/Node.hpp>
#include <xdm/PrimitiveType.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// A Data is a class that holds large datasets.
class Data : public Node {
public:
  Data();
  virtual ~Data();
};

XDM_NAMESPACE_END

#endif // xdm_Data_hpp
