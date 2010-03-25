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
#ifndef xdmExodus_Writer_hpp
#define xdmExodus_Writer_hpp

#include <xdmFormat/Writer.hpp>

#include <xdmExodus/NamespaceMacro.hpp>

XDM_EXODUS_NAMESPACE_BEGIN

/// Class for writing an ExodusII file. Uses the ExodusII library functions to write
/// an unstructured grid to an ExodusII file complete with nodes and blocks for now.
class ExodusWriter : public xdmFormat::Writer {
public:
  ExodusWriter();
  virtual ~ExodusWriter();

  /// Write an Item to the file with the given path.
  /// @see xdmFormat::Writer::writeItem
  virtual void writeItem( xdm::RefPtr< xdm::Item > item, const xdm::FileSystemPath& path );

  /// Write an existing Item and it's subtree at a new time step. If there is no dynamic
  /// data in the tree, then this function does nothing.
  /// @see xdmFormat::Writer::update
  virtual bool update(
    xdm::RefPtr< xdm::Item > item,
    const xdm::FileSystemPath& path,
    std::size_t timeStep = 0 );

};

XDM_EXODUS_NAMESPACE_END

#endif // xdmExodus_Writer_hpp

