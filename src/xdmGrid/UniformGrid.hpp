//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmGrid_UniformGrid_hpp
#define xdmGrid_UniformGrid_hpp

#include <xdmGrid/Attribute.hpp>
#include <xdmGrid/Grid.hpp>

#include <xdm/ObjectCompositionMixin.hpp>
#include <xdm/PrimitiveType.hpp>
#include <xdm/RefPtr.hpp>

#include <string>
#include <vector>

#include <xdm/NamespaceMacro.hpp>
#include <xdmGrid/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN
  class ItemVisitor;
XDM_NAMESPACE_END

XDM_GRID_NAMESPACE_BEGIN

class Geometry;
class Topology;

/// Grid type containing the actual geometry and topology for a grid.  This is a
/// terminal grid node that contains the geometric and topological properties of
/// a Grid along with any attributes defined on the grid.
class UniformGrid : 
  public Grid,
  public xdm::ObjectCompositionMixin< Attribute > {
public:
  UniformGrid();
  virtual ~UniformGrid();

  XDM_META_ITEM( UniformGrid );

  void setGeometry( Geometry* geo );
  Geometry* geometry();
  const Geometry* geometry() const;

  void setTopology( Topology* topo );
  Topology* topology();
  const Topology* topology() const;

  /// Add an attribute definition to the grid.
  void addAttribute( Attribute* attribute );

  /// Redefinition of visitor traversal from xdm::Item.
  virtual void traverse( xdm::ItemVisitor& iv );

  /// Redefinition of metadata from Grid.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  xdm::RefPtr< Geometry > mGeometry;
  xdm::RefPtr< Topology > mTopology;
};

/// Construct an attribute on a uniform grid with the given center.  
///
/// This non-member function will construct the attribute definition with a
/// corresponding UniformDataItem to hold the attribute data.  The attribute is
/// not automatically added to the grid, it is up to clients to do that.
xdm::RefPtr< xdmGrid::Attribute >
createAttribute( 
  const UniformGrid* grid, 
  Attribute::Center center,
  Attribute::Type type,
  const std::string& name,
  xdm::primitiveType::Value dataType );

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_UniformGrid_hpp

