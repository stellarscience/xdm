//=============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//-----------------------------------------------------------------------------
#ifndef xdmGrid_Geometry_hpp
#define xdmGrid_Geometry_hpp

#include <xdm/DataItem.hpp>
#include <xdm/Item.hpp>
#include <xdm/ObjectCompositionMixin.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Base class for descriptions of the geometric properties of a grid.  This
/// class is intended to be a base class that all different types of geometric
/// description of grid attributes will inherit from.
class Geometry : 
  public xdm::Item,
  public xdm::ObjectCompositionMixin< xdm::DataItem > {
public:
  /// Constructor takes the dimension of the space.  That is, the size of a
  /// maximal set of linearly independent vectors in the space, and thus the
  /// number of components required to identify a point in space.
  Geometry( unsigned int dimension );
  virtual ~Geometry();

  XDM_META_ITEM( Geometry );

  /// Set the dimension of the space.
  void setDimension( unsigned int dimension );
  /// Get the dimension of the space.
  unsigned int dimension() const;

  virtual void traverse( xdm::ItemVisitor& iv );

  /// Write geometry metadata.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

private:
  unsigned int mDimension;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Geometry_hpp

