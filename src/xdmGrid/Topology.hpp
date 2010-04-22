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
#ifndef xdmGrid_Topology_hpp
#define xdmGrid_Topology_hpp

#include <xdmGrid/ElementTopology.hpp>

#include <xdm/DataItem.hpp>
#include <xdm/Item.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/ObjectCompositionMixin.hpp>
#include <xdm/VectorRef.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

typedef xdm::VectorRef< std::size_t > ElementConnectivity;
typedef xdm::ConstVectorRef< std::size_t > ConstElementConnectivity;

/// Base class for topological descriptions of a grid.  The topological
/// properties of a grid are those which are invariant under rotations,
/// translations, and scale.  Connectivity is the most important topological
/// property.
///
/// A Grid consists of elements (or Elements) and nodes.
class Topology :
  public xdm::Item,
  public xdm::ObjectCompositionMixin< xdm::DataItem > {
public:
  Topology();
  virtual ~Topology();

  /// Get the node odering for the shape of these Elements.
  virtual NodeOrderingConvention::Type nodeOrdering() const = 0;

  /// Set the number of Elements in the topology.
  void setNumberOfElements( std::size_t numberOfElements );
  /// Get the number of Elements in the topology.
  virtual std::size_t numberOfElements() const;

  /// Get the const connectivity of a single Element.
  ConstElementConnectivity elementConnections( std::size_t elementIndex ) const;

  /// Get the type of a particular Element.
  virtual xdm::RefPtr< const ElementTopology > elementTopology(
    const std::size_t& elementIndex ) const = 0;

  XDM_META_ITEM( Topology );

  virtual void traverse( xdm::ItemVisitor& iv );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

protected:
  /// Construct the vector implementation for node IDs used by this topology.
  /// Subclasses should override this method to return an appropriate VectorRefImp
  /// for their internal Element representation.
  virtual xdm::RefPtr< xdm::VectorRefImp< std::size_t > > createVectorImp() = 0;

private:
  xdm::RefPtr< xdm::VectorRefImp< std::size_t > > mSharedVectorImp;
  std::size_t mNumberOfElements;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Topology_hpp

