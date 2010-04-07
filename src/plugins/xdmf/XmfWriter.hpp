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
#ifndef xdmf_XmfWriter_hpp
#define xdmf_XmfWriter_hpp

#include <xdmFormat/Writer.hpp>

#include <map>

#include <xdmf/NamespaceMacro.hpp>

XDMF_NAMESPACE_BEGIN

class TimeSeries;

/// Implementation of xdmFormat::Writer for writing XDMF files.
class XmfWriter : public xdmFormat::Writer {
public:
  XmfWriter();
  virtual ~XmfWriter();

  /// Write an item to and XDMF file.
  virtual void writeItem( xdm::RefPtr< xdm::Item > item, const xdm::FileSystemPath& path );

  /// Write an existing Item at a new time step;
  virtual bool update(
    xdm::RefPtr<xdm::Item>item,
    const xdm::FileSystemPath &path,
    std::size_t timeStep );

private:
  typedef std::map< xdm::FileSystemPath, xdm::RefPtr< TimeSeries > > FileMap;
  FileMap mWriters;
};

XDMF_NAMESPACE_END

#endif // xdmf_XmfWriter_hpp
