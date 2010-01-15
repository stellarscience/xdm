#ifndef xdmGrid_CollectionGrid_hpp
#define xdmGrid_CollectionGrid_hpp

#include <xdmGrid/Grid.hpp>

#include <xdm/ObjectCompositionMixin.hpp>
#include <xdm/RefPtr.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Grid type that contains other Grids.  The collection can be spatial or
/// temporal, that is a collection of multiple grids in different locations, or
/// a collection of grids representing the simulation at different points in
/// time.
class CollectionGrid : 
  public Grid,
  public xdm::ObjectCompositionMixin< Grid > {
public:

  /// Enumeration of collection type.  Spatial or temporal.
  enum CollectionType {
    kSpatial = 0,
    kTemporal
  };

  explicit CollectionGrid( CollectionType type = kSpatial );
  virtual ~CollectionGrid();

  XDM_META_ITEM( CollectionGrid );

  /// Set the collection type for this grid to spatial or temporal.
  /// @see CollectionType
  void setType( CollectionType t );
  CollectionType type() const;

  /// Definition of visitor traversal.
  virtual void traverse( xdm::ItemVisitor& iv );

  /// Definition of metadata output.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );
private:
  CollectionType mType;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_CollectionGrid_hpp

