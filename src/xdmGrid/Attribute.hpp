#ifndef xdmGrid_Attribute_hpp
#define xdmGrid_Attribute_hpp

#include <xdm/Item.hpp>
#include <xdm/ItemVisitor.hpp>
#include <xdm/RefPtr.hpp>

#include <xdmGrid/NamespaceMacro.hpp>
#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN
  class UniformDataItem;
XDM_NAMESPACE_END

XDM_GRID_NAMESPACE_BEGIN

class Attribute : public xdm::Item {
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

  /// Set the attribute's data structure and type.
  void setDataItem( xdm::UniformDataItem* item );
  /// Get the attribute's data structure and type.
  xdm::UniformDataItem* dataItem();
  /// Get the attribute's const data structure and type.
  const xdm::UniformDataItem* dataItem() const;
  

  virtual void traverse( xdm::ItemVisitor& iv );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  Type mType;
  Center mCenter;
  xdm::RefPtr< xdm::UniformDataItem > mDataItem;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Attribute_hpp

