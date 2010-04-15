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

#include <xdmf/TimeSeries.hpp>

#include <xdmFormat/Writer.hpp>

#include <map>

#include <xdmf/NamespaceMacro.hpp>

XDMF_NAMESPACE_BEGIN

/// Implementation of xdmFormat::Writer for writing XDMF files.
class XmfWriter : public xdmFormat::Writer {
public:
  XmfWriter();
  virtual ~XmfWriter();

  virtual void open(
    const xdm::FileSystemPath& path,
    xdm::Dataset::InitializeMode mode );
  virtual void write( xdm::RefPtr< xdm::Item > item );
  virtual void close();

private:
  xdm::RefPtr< TimeSeries > mSeries;
  bool mIsOpen;
  xdm::FileSystemPath mCurrentFilePath;
};

XDMF_NAMESPACE_END

#endif // xdmf_XmfWriter_hpp
