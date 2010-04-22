//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009-2010 Stellar Science. Government-purpose rights granted.
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
#ifndef xdmGrid_UnstructuredTopology_hpp
#define xdmGrid_UnstructuredTopology_hpp

#include <xdmGrid/ElementTopology.hpp>
#include <xdmGrid/Topology.hpp>

#include <xdm/RefPtr.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Base class for all unstructured topologies. If the connectivity is different
/// from the standard, an order may be specified.
class UnstructuredTopology : public Topology {
public:
  UnstructuredTopology();
  virtual ~UnstructuredTopology();

  XDM_META_ITEM( UnstructuredTopology );

  /// Set the type of the Elements.
  void setElementTopology( xdm::RefPtr< const ElementTopology > topo );

  /// Get the type of the Elements. For now, this ignores the elementIndex and assumes all
  /// Elements are of the same type.
  virtual xdm::RefPtr< const ElementTopology > elementTopology(
    const std::size_t& elementIndex ) const;

  /// Set the node ordering convention for the shape of the Elements. For example,
  /// Exodus II orders the nodes of a linear tetrahedron differently than some
  /// other formats.
  void setNodeOrdering( const NodeOrderingConvention::Type& order );
  /// Get the node odering for the shape of these Elements.
  NodeOrderingConvention::Type nodeOrdering() const;

  /// Set the connectivity values to the input DataItem. If the connectivity
  /// is not specified, then there is a default connectivity determined by the
  /// topology type.
  void setConnectivity( xdm::RefPtr< xdm::UniformDataItem > connectivity );

  virtual void traverse( xdm::ItemVisitor& iv );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );

protected:
  virtual xdm::RefPtr< xdm::VectorRefImp< std::size_t > > createVectorImp();

private:
  xdm::RefPtr< xdm::UniformDataItem > mConnectivity;
  xdm::RefPtr< const ElementTopology > mElementTopology;
  NodeOrderingConvention::Type mOrdering;
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_UnstructuredTopology_hpp

