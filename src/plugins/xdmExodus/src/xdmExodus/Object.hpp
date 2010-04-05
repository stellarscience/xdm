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
#ifndef xdmExodus_Object_hpp
#define xdmExodus_Object_hpp

#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

class ExodusString;
class Variable;

/// An Exodus object refers to an array of values describing a block or set in a NetCDF file.
/// Nodes, sets, and blocks are all objects. Exodus properties, attributes, and variables can
/// be assigned to objects.
class Object {
public:
  int id() const;
  void setId( int id );

  int exodusObjectType() const;

  int exodusInquireFlag() const;

  const char* exodusTypeChar() const;

  virtual std::size_t numberOfEntries() const = 0;

  /// Add a Variable to the Object.
  /// Inheriting classes should call this function if the variables() function is to ever
  /// be called. This function just stashes the pointer for future use to avoid downcasting
  /// xdmGrid::Attributes to Variables.
  virtual void addVariable( xdm::RefPtr< Variable > variable );

  std::vector< xdm::RefPtr< Variable > > variables();

  void setupVariables(
    std::vector< int >::const_iterator beginTruthTable,
    const std::size_t numberOfVariables,
    const std::vector< ExodusString >& variableNames );

  virtual void writeToFile( int exodusFileId, int* variableTruthTable ) {}

  /// Read the data for the variables at a specific time step.
  /// @pre A call to setupVariables to make sure the variables exist in the data structure.
  /// @post The Variables in the Object have values corresponding to @arg timeStep.
  virtual void readTimeStep( int exodusFileId, std::size_t timeStep );

  virtual void writeTimeStep( int exodusFileId, std::size_t timeStep );

protected:
  /// Inheriting classes must return the appropriate index into the xdmExodus::kObjectTypes array.
  virtual int exodusObjectTypeIndex() const = 0;

private:
  std::vector< xdm::RefPtr< Variable > > mVariables;
  int mId;
};

XDM_EXODUS_NAMESPACE_END

#endif // xdmExodus_Object_hpp

