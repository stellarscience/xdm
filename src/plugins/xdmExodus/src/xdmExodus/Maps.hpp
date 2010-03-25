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
#ifndef xdmExodus_Maps_hpp
#define xdmExodus_Maps_hpp

#include <xdmExodus/Object.hpp>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

/// An Exodus map is not really an object, but it inherits from Object to grab some of
/// the Exodus type lookup functionality.
class Map :
  public Object {
};

class NodeMap : public Map {
protected:
  virtual int exodusObjectTypeIndex() const { return 8; }
};

class EdgeMap : public Map {
protected:
  virtual int exodusObjectTypeIndex() const { return 9; }
};

class FaceMap : public Map {
protected:
  virtual int exodusObjectTypeIndex() const { return 10; }
};

class ElementMap : public Map {
protected:
  virtual int exodusObjectTypeIndex() const { return 11; }
};

XDM_EXODUS_NAMESPACE_END

#endif // xdmExodus_Maps_hpp

