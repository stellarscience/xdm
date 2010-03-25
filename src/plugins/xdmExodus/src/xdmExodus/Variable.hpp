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
#ifndef xdmExodus_Variable_hpp
#define xdmExodus_Variable_hpp

#include <xdmGrid/Attribute.hpp>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN
template< typename T > class VectorStructuredArray;
XDM_NAMESPACE_END

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

class Variable : public xdmGrid::Attribute {
public:
  Variable(
    int exodusObjectType,
    int variableIndex,
    int objectId,
    std::size_t numberOfEntries );

  void readTimeStep( int exodusFileId, std::size_t timeStep );

  int id() const;

private:
  int mExodusObjectType;
  int mVariableIndex;
  int mObjectId;
};

XDM_EXODUS_NAMESPACE_END

#endif // xdmExodus_Variable_hpp

