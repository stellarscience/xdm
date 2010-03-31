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
#include <xdm/MemoryAdapter.hpp>

#include <xdm/Dataset.hpp>

XDM_NAMESPACE_BEGIN

MemoryAdapter::MemoryAdapter( bool isDynamic ) :
  ReferencedObject(),
  mIsDynamic( isDynamic ),
  mNeedsUpdate( true )
{
}

MemoryAdapter::~MemoryAdapter()
{
}

bool MemoryAdapter::isDynamic() const
{
  return mIsDynamic;
}

void MemoryAdapter::setIsDynamic( bool isDynamic )
{
  mIsDynamic = isDynamic;
}

bool MemoryAdapter::needsUpdate() const
{
  return mNeedsUpdate;
}

void MemoryAdapter::setNeedsUpdate( bool needsUpdate )
{
  mNeedsUpdate = needsUpdate;
}

bool MemoryAdapter::requiresWrite() const {
  return ( mIsDynamic || mNeedsUpdate );
}

void MemoryAdapter::write( Dataset* dataset )
{
  if ( requiresWrite() ) {
    writeImplementation( dataset );
    mNeedsUpdate = false;
  }
}

RefPtr< StructuredArray > MemoryAdapter::array() {
  return const_pointer_cast< StructuredArray >(
    static_cast< const MemoryAdapter& >( *this ).array() );
}

XDM_NAMESPACE_END

