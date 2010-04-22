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
#ifndef xdm_Element_hpp
#define xdm_Element_hpp

#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/ElementTopology.hpp>

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// A shared lookup class for Elements. Elements refer connectivity and geometry queries to this
/// shared class.
class ElementSharedConnectivityLookup : public xdm::ReferencedObject {
public:

  /// Get the geometry node index for a particular element and local node index.
  /// @param elementIndex The index of a particular element in a particular topology.
  /// @param nodeIndex The local index of a node on the element (for example, a triangle might
  ///        have three nodes, with indices 0, 1, and 2.
  /// @returns The node index into a geometry that corresponds to the local element node index.
  virtual std::size_t at( const std::size_t& elementIndex, const std::size_t& nodeIndex ) const = 0;

  /// Get the geometry that pertains to a particular element in the topology.
  virtual const Geometry& geometry( const std::size_t& elementIndex ) const = 0;
};

/// A lightweight class that provides useful functions for element access, but that does not hold
/// onto any global connectivity data. All functions are const, which means all Elements are const.
class Element {
public:
  /// @brief Constructor.
  /// @param connections The shared connectivity class that will be used to lookup nodes in a
  ///        big topology connectivity array.
  /// @param elementTopology The topology of this element. Use xdmGrid::elementFactory() to grab
  ///        the relevant topology.
  /// @param elementIndex The index of this element that will be used when making calls to the
  ///        shared connectivity class.
  Element(
    xdm::RefPtr< const ElementSharedConnectivityLookup > connections,
    xdm::RefPtr< const ElementTopology > elementTopology,
    const std::size_t& elementIndex ) :
      mConnectivity( connections ), mTopo( elementTopology ), mIndex( elementIndex ) {
  }

  /// Get a ConstNode that corresponds to a local node index on the element.
  ConstNode node( const std::size_t& nodeIndex ) const {
    return mConnectivity->geometry( mIndex ).node( nodeIndexInGeometry( nodeIndex ) );
  }

  /// Get the type of shape.
  ElementShape::Type shape() const {
    return mTopo->shape();
  }

  /// Get the number of faces. This will be zero if the element is not a volume element.
  std::size_t numberOfFaces() const {
    return mTopo->numberOfFaces();
  }

  /// Get the number of edges. This will be zero if the element is not a volume or surface element.
  std::size_t numberOfEdges() const {
    return mTopo->numberOfEdges();
  }

  /// Get the number of nodes on this element.
  std::size_t numberOfNodes() const {
    return mTopo->numberOfNodes();
  }

  /// Get a particular face. The returned Element has full Element capabilities, including the
  /// ability to export its edges as Elements. Any node lookups for this face or any of its
  /// edges will refer to nodes from the original volume element, and these can be mapped back
  /// via Element::nodeIndexInGeometry() to give the correct global node indices.
  Element face( const std::size_t& faceIndex ) const {
    return Element( mConnectivity, mTopo->face( faceIndex ), mIndex );
  }

  /// Get a particular edge. Any node lookups for this edge will refer to nodes from the original
  /// volume or face element, and these can be mapped back via Element::nodeIndexInGeometry() to
  /// give the correct global node indices.
  Element edge( const std::size_t& edgeIndex ) const {
    return Element( mConnectivity, mTopo->edge( edgeIndex ), mIndex );
  }

  /// Get the global node index into the geometry that corresponds to a local node index for this
  /// element.
  std::size_t nodeIndexInGeometry( const std::size_t& localNodeIndex ) const {
    return mConnectivity->at( mIndex, mTopo->node( localNodeIndex ) );
  }

private:
  xdm::RefPtr< const ElementSharedConnectivityLookup > mConnectivity;
  xdm::RefPtr< const ElementTopology > mTopo;
  std::size_t mIndex;
};

XDM_GRID_NAMESPACE_END

#endif // xdm_Element_hpp
