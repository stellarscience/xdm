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
#ifndef xdmf_XmfReader_hpp
#define xdmf_XmfReader_hpp

#include <xdmFormat/Reader.hpp>

#include <xdmf/NamespaceMacro.hpp>

#include <memory>
#include <sstream>

XDMF_NAMESPACE_BEGIN

class XmfReader : public xdmFormat::Reader {
public:
  XmfReader();
  virtual ~XmfReader();

  virtual xdmFormat::Reader::ReadResult readItem(
    const xdm::FileSystemPath& path );

  virtual bool update(
    xdm::RefPtr< xdm::Item > item,
    const xdm::FileSystemPath& path,
    std::size_t timeStep = 0 );

private:
  // This class uses a private implementation to keep LibXml2 out of the
  // header.
  class Private;
  std::auto_ptr< Private > mImp;
};

XDMF_NAMESPACE_END

#endif // xdmf_XmfReader_hpp
