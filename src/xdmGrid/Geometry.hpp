//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009 Stellar Science. Government-purpose rights granted.
//
// This file is part of XDM
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//------------------------------------------------------------------------------
#ifndef xdmGrid_Geometry_hpp
#define xdmGrid_Geometry_hpp

#include <xdm/UniformDataItem.hpp>
#include <xdm/Item.hpp>
#include <xdm/ObjectCompositionMixin.hpp>
#include <xdm/VectorRef.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class NodeRef;

/// Base class for descriptions of the geometric properties of a grid.  This
/// class is intended to be a base class that all different types of geometric
/// description of grid attributes will inherit from.
class Geometry :
  public xdm::Item,
  public xdm::ObjectCompositionMixin< xdm::UniformDataItem > {
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

  /// Set the number of nodes (coordinate values).
  void setNumberOfNodes( std::size_t n );
  /// Get the number of nodes.
  std::size_t numberOfNodes() const;

  /// Get a shared node by index.
  NodeRef node( std::size_t nodeIndex );
  /// Get a constant shared node by index.
  const NodeRef node( std::size_t nodeIndex ) const;

  virtual void traverse( xdm::ItemVisitor& iv );

  /// Write geometry metadata.
  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

protected:

  /// Construct the vector implementation used by this geometry. Subclasses
  /// should override this method to return an appropriate VectorRefImpl for
  /// their internal geometry representation.
  virtual xdm::RefPtr< xdm::VectorRefImp< double > > createVectorImp() = 0;

private:
  std::size_t mNumberOfNodes;
  unsigned int mDimension;
  xdm::RefPtr< xdm::VectorRefImp< double > > mSharedVectorImp;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Geometry_hpp

