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
#include <xdmGrid/NodeRef.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

NodeRef::NodeRef( xdm::VectorRef< double > location ) :
  mLocation( location )
{
}

NodeRef::NodeRef( const NodeRef& copyMe ) :
  mLocation( copyMe.mLocation )
{
}

NodeRef& NodeRef::operator=( const NodeRef& rhs )
{
  if ( &rhs != this ) {
    mLocation = rhs.mLocation;
  }
  return *this;
}

xdm::VectorRef< double >& NodeRef::location()
{
  return mLocation;
}

const xdm::VectorRef< double >& NodeRef::location() const
{
  return mLocation;
}

XDM_GRID_NAMESPACE_END

