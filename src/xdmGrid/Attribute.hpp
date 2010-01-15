#ifndef xdmGrid_Attribute_hpp
#define xdmGrid_Attribute_hpp

#include <xdm/DataItem.hpp>
#include <xdm/Item.hpp>
#include <xdm/ObjectCompositionMixin.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Attribute : 
  public xdm::Item,
  public xdm::ObjectCompositionMixin< xdm::DataItem > {
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

  XDM_META_ITEM( Attribute );

  virtual void traverse( xdm::ItemVisitor& iv );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  Type mType;
  Center mCenter;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Attribute_hpp

