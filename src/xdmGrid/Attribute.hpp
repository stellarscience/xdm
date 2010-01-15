#ifndef xdmGrid_Attribute_hpp
#define xdmGrid_Attribute_hpp

#include <xdm/CompositeDataItem.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Attribute : public xdm::CompositeDataItem {
public:
  /// Enumeration of attribute types.
  enum Type {
    kScalar = 0,
    kVector,
    kTensor,
    kTensor6,
    kMatrix
  };

  /// Enumeration of attribute centering.
  enum Center {
    kNode = 0,
    kEdge,
    kFace,
    kCell,
    kGrid
  };

  Attribute( Type t = kScalar, Center c = kNode );
  virtual ~Attribute();

public:
  Type mType;
  Center mCenter;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Attribute_hpp

