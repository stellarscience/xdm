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
#ifndef xdmExodus_ExodusReader_hpp
#define xdmExodus_ExodusReader_hpp

#include <xdmFormat/Reader.hpp>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

/// Class for reading an ExodusII file. Uses the ExodusII library functions to read
/// an unstructured grid from an ExodusII file complete with nodes and element blocks
/// for now.
class ExodusReader : public xdmFormat::Reader {
public:
  ExodusReader();
  virtual ~ExodusReader();

  /// Read an Item from a give file with the given name.
  virtual xdm::RefPtr< xdm::Item > readItem( const std::string& filename );
};

XDM_EXODUS_NAMESPACE_END

#endif // xdmExodus_ExodusReader_hpp

