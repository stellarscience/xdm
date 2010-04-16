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
#include <xdmExodus/Variable.hpp>
#include <xdmExodus/Helpers.hpp>

#include <xdm/ArrayAdapter.hpp>
#include <xdm/VectorStructuredArray.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

Variable::Variable(
  int exodusObjectType,
  int variableIndex,
  int objectId,
  std::size_t numberOfEntries ) :

  xdmGrid::Attribute( xdmGrid::Attribute::kScalar, xdmGrid::Attribute::kElement ),
  mExodusObjectType( exodusObjectType ),
  mVariableIndex( variableIndex ),
  mObjectId( objectId ) {

  xdm::RefPtr< xdm::VectorStructuredArray< double > > data(
    new xdm::VectorStructuredArray< double >( numberOfEntries ) );
  xdm::RefPtr< xdm::UniformDataItem > dataItem(
    new xdm::UniformDataItem( xdm::primitiveType::kDouble, xdm::makeShape( numberOfEntries ) ) );
  dataItem->setData( xdm::makeRefPtr( new xdm::ArrayAdapter( data ) ) );
  dataItem->data()->setIsDynamic( true );
  setDataItem( dataItem );
}

void Variable::readTimeStep( int exodusFileId, std::size_t timeStep ) {
  EXODUS_CALL(
    ex_get_var(
      exodusFileId,
      (int)( timeStep + 1 ),
      mExodusObjectType,
      mVariableIndex,
      mObjectId,
      (int)dataItem()->data()->array()->size(),
      dataItem()->data()->array()->data() ),
    "Could not read variable values." );
}

void Variable::writeTimeStep( int exodusFileId, std::size_t timeStep ) {
  EXODUS_CALL(
    ex_put_var(
      exodusFileId,
      (int)( timeStep + 1 ),
      mExodusObjectType,
      mVariableIndex,
      mObjectId,
      (int)dataItem()->data()->array()->size(),
      (void*)dataItem()->typedArray< double >()->begin() ),
    "Unable to write variable." );
}

int Variable::id() const {
  return mVariableIndex;
}

XDM_EXODUS_NAMESPACE_END
