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
#ifndef xdm_Cell_hpp
#define xdm_Cell_hpp

#include <xdmGrid/Geometry.hpp>
#include <xdmGrid/UnstructuredTopologyConventions.hpp>

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

class Cell;
class ConstCell;
class CellSharedImp;

class CellBase
{
public:
  CellBase( xdm::RefPtr< CellSharedImp > imp, std::size_t cellIndex );

  /// Get a single const node.
  ConstNode node( std::size_t nodeIndex ) const;

  /// The type of cell.
  CellType::Type cellType() const;

protected:
  CellBase( const CellBase& copyMe );

  static void copyReference( const CellBase& source, CellBase& dest );

private:
  xdm::RefPtr< CellSharedImp > mImp;
  std::size_t mIndex;
};

/// A cell class that references data in a presumably large dataset. The Cell acts as
/// self-contained cell, but it actually just refers most requests to an implementation
/// class. Thus, the cell is lightweight and copying is cheap.
class Cell : public CellBase
{
public:
  Cell( xdm::RefPtr< CellSharedImp > imp, std::size_t cellIndex );
  Cell( const Cell& copyMe );

  /// Get a single node.
  Node node( std::size_t nodeIndex );

private:
  xdm::RefPtr< CellSharedImp > mImp;
  std::size_t mIndex;

  // Referencing a const cell with a non-const cell is not safe.
  Cell( const ConstCell& copyMe );
};

/// Const version of a Cell.
class ConstCell : public CellBase
{
public:
  ConstCell( xdm::RefPtr< CellSharedImp > imp, std::size_t cellIndex );
  ConstCell( const Cell& copyMe );
  ConstCell( const ConstCell& copyMe );

  /// This is intended to be a const_cast for Cells.
  Cell removeConstness() const;
private:
  xdm::RefPtr< CellSharedImp > mImp;
  std::size_t mIndex;

  // No non-const node access.
  Node node( std::size_t nodeIndex );
};

/// Shared implementation class for referencing Cell data that is not stored in a self-contained
/// Cell structure. All Cells that use a given shared implementation should be of the same
/// type, have the same number of nodes, etc. However, that is not required.
class CellSharedImp : public xdm::ReferencedObject
{
public:
  /// Get a single node.
  virtual ConstNode node( std::size_t cellIndex, std::size_t nodeIndex ) const = 0;

  /// The cell type for all cells that this shared implementation refers to.
  virtual CellType::Type cellType( std::size_t cellIndex ) const = 0;
};

XDM_GRID_NAMESPACE_END

#endif // xdm_Cell_hpp
