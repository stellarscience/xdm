//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2010 Stellar Science. Government-purpose rights granted.
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

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Node;

/// Shared implementation class for referencing element data that is not stored in a self-contained
/// element structure.
class ElementImpl : public xdm::ReferencedObject
{
public:
  /// Get a single node (the Node class is a reference class).
  virtual const Node node( std::size_t nodeIndex, std::size_t elementIndex ) const;

  /// The number of nodes for all elements that this shared implementation refers to.
  virtual std::size_t numberOfNodes() const;
};

class Element
{
public:
  Element( xdm::RefPtr< ElementImpl > imp, std::size_t elementIndex );

  /// Get a single const node (the Node class is a reference class).
  const Node node( std::size_t nodeIndex ) const;
  /// Get a single node (the Node class is a reference class).
  Node node( std::size_t nodeIndex );

  /// The number of total nodes for this element.
  std::size_t numberOfNodes() const;

private:
  xdm::RefPtr< ElementImpl > mImp;
  std::size_t mIndex;
};

XDM_GRID_NAMESPACE_END

#endif // xdm_Element_hpp
