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
#ifndef xdm_NodeRef_hpp
#define xdm_NodeRef_hpp

#include <xdm/VectorRef.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// A lightweight class that represents a single node in a geometry. The Node keeps
/// track of its location, which is represented by a VectorRef. This means
/// that the Node itself is a reference object, and thus, changes to a Node's location
/// are propagated back through to the original dataset.
class NodeRef
{
public:
  NodeRef( xdm::VectorRef< double > location );
  NodeRef( const NodeRef& copyMe );

  NodeRef& operator=( const NodeRef& rhs );

  xdm::VectorRef< double >& location();
  const xdm::VectorRef< double >& location() const;

private:
  xdm::VectorRef< double > mLocation;
};

XDM_GRID_NAMESPACE_END

#endif // xdm_NodeRef_hpp

