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
#ifndef xdmExodus_Blocks_hpp
#define xdmExodus_Blocks_hpp

#include <xdmExodus/Object.hpp>

#include <xdmGrid/UniformGrid.hpp>

#include <xdm/Forward.hpp>

#include <string>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

class Variable;

/// An Exodus block is an integer array of offsets into the nodal coordinate array. The offsets
/// refer to the nodal ordering of the entries in the block. There are edge, face, and element
/// blocks.
class Block :
  public Object,
  public xdmGrid::UniformGrid {
public:
  std::size_t entryGlobalOffset() const;
  void setEntryGlobalOffset( std::size_t offset );

  virtual std::size_t numberOfEntries() const;

  virtual void addVariable( xdm::RefPtr< Variable > variable );

  /// Get the Exodus attributes (note that these are a subset of xdmGrid::Attributes because
  /// Exodus uses the term "variable" to denote dynamic attributes).
  virtual std::vector< xdm::RefPtr< xdmGrid::Attribute > > attributes();

  virtual void readFromFile(
    int exodusFileId,
    int exodusObjectId,
    std::string blockName,
    xdm::RefPtr< xdmGrid::Geometry > geom,
    std::vector< int >::const_iterator beginTruthTable,
    const std::size_t numberOfVariables,
    const std::vector< ExodusString >& variableNames );

  virtual void writeToFile( int exodusFileId, int* variableTruthTable );

protected:
  virtual void readAttributes( int exodusFileId, std::size_t attributesPerEntry );
  virtual void writeAttributes( int exodusFileId );

private:
  std::size_t mOffset;
};

class EdgeBlock : public Block {
protected:
  virtual int exodusObjectTypeIndex() const { return 0; }
};

class FaceBlock : public Block {
protected:
  virtual int exodusObjectTypeIndex() const { return 1; }
};

class ElementBlock : public Block {
protected:
  virtual int exodusObjectTypeIndex() const { return 2; }
};

/// Returns the Block class that corresponds to an Exodus block type.
xdm::RefPtr< Block > blockFactory( int exodusObjectType );

XDM_EXODUS_NAMESPACE_END

#endif // xdmExodus_Blocks_hpp

