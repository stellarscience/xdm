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
#ifndef xdmExodus_Sets_hpp
#define xdmExodus_Sets_hpp

#include <xdmExodus/Object.hpp>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

/// An Exodus set is an integer array of offsets into internal element, face, edge, or node
/// IDs. Note that these are *internal* IDs, which can only be determined by reading the
/// elements/faces/edges sequentially from file and numbering them beginning with 1.
class Set :
  public Object {
};

class NodeSet : public Set {
protected:
  virtual int exodusObjectTypeIndex() const { return 3; }
};

class EdgeSet : public Set {
protected:
  virtual int exodusObjectTypeIndex() const { return 4; }
};

class FaceSet : public Set {
protected:
  virtual int exodusObjectTypeIndex() const { return 5; }
};

class SideSet : public Set {
protected:
  virtual int exodusObjectTypeIndex() const { return 6; }
};

class ElementSet : public Set {
protected:
  virtual int exodusObjectTypeIndex() const { return 7; }
};

XDM_EXODUS_NAMESPACE_END

#endif // xdmExodus_Sets_hpp

