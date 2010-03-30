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

#include <xdmExodus/Object.hpp>
#include <xdmExodus/Helpers.hpp>
#include <xdmExodus/Variable.hpp>

#include <xdm/RefPtr.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

class Variable;

int Object::id() const { return mId; }
void Object::setId( int id ) { mId = id; }

int Object::exodusObjectType() const { return kObjectTypes[ exodusObjectTypeIndex() ]; }

int Object::exodusInquireFlag() const { return kInquireObjectSizes[ exodusObjectTypeIndex() ]; }

const char* Object::exodusTypeChar() const { return kObjectTypeChar[ exodusObjectTypeIndex() ]; }

void Object::setupVariables(
  std::vector< int >::const_iterator beginTruthTable,
  const std::size_t numberOfVariables,
  const std::vector< ExodusString >& variableNames ) {

  for ( std::size_t variableIndex = 0; variableIndex < numberOfVariables; ++variableIndex ) {
    if ( *beginTruthTable++ == 0 ) {
      continue;
    }

    xdm::RefPtr< Variable > variable( new Variable(
      exodusObjectType(),
      variableIndex,
      id(),
      numberOfEntries() ) );
    variable->setName( variableNames[ variableIndex ].string() );
    addVariable( variable );

    // Keep a copy of the variable as a Variable to avoid having to dynamic cast
    // xdmGrid::Attributes to Variables later when reading time step data.
    mVariables.push_back( variable );
  }
}

void Object::readTimeStep( int exodusFileId, std::size_t timeStep ) {
  typedef std::vector< xdm::RefPtr< Variable > >::iterator VariableIterator;
  for ( VariableIterator var = mVariables.begin(); var != mVariables.end(); ++var ) {
    (*var)->readTimeStep( exodusFileId, timeStep );
  }
}

void Object::writeTimeStep( int exodusFileId, std::size_t timeStep ) {
  typedef std::vector< xdm::RefPtr< Variable > >::iterator VariableIterator;
  for ( VariableIterator var = mVariables.begin(); var != mVariables.end(); ++var ) {
    (*var)->writeTimeStep( exodusFileId, timeStep );
  }
}

XDM_EXODUS_NAMESPACE_END
