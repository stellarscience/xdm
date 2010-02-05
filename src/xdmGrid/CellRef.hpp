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
#ifndef xdm_CellRef_hpp
#define xdm_CellRef_hpp

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class NodeRef;

/// Shared implementation class for referencing Cell data that is not stored in a self-contained
/// Cell structure. All Cells that use a given shared implementation should be of the same
/// type, have the same number of nodes, etc.
class CellSharedImp : public xdm::ReferencedObject
{
public:
  /// Get a single node (the NodeRef class is a reference class).
  virtual const NodeRef node( std::size_t nodeIndex, std::size_t CellIndex ) const;

  /// The number of nodes for all Cells that this shared implementation refers to.
  virtual std::size_t numberOfNodes() const;
};

class CellRef
{
public:
  CellRef( xdm::RefPtr< CellSharedImp > imp, std::size_t CellIndex );
  CellRef( const CellRef& copyMe );

  CellRef& operator=( const CellRef& rhs );

  /// Get a single const node (the NodeRef class is a reference class).
  const NodeRef node( std::size_t nodeIndex ) const;
  /// Get a single node (the NodeRef class is a reference class).
  NodeRef node( std::size_t nodeIndex );

  /// The number of total nodes for this Cell.
  std::size_t numberOfNodes() const;

private:
  xdm::RefPtr< CellSharedImp > mImp;
  std::size_t mIndex;
};

XDM_GRID_NAMESPACE_END

#endif // xdm_CellRef_hpp