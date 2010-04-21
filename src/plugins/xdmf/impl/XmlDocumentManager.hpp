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
#ifndef xdmf_impl_NodeList_hpp
#define xdmf_impl_NodeList_hpp

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <libxml/tree.h>

#include <vector>

namespace xdmf {
namespace impl {

/// A reference counted std vector of xmlNodes.
class SharedNodeVector :
  public xdm::ReferencedObject,
  private std::vector< xmlNode * >
{
  typedef std::vector< xmlNode * > VectorBase;
public:
  SharedNodeVector() : VectorBase() {}
  virtual ~SharedNodeVector() {}

  using VectorBase::push_back;
  using VectorBase::operator[];
  using VectorBase::size;
  using VectorBase::at;
};

/// A reference counted RAII class for an XML document.
class XmlDocumentManager : public xdm::ReferencedObject {
public:
  XmlDocumentManager( xmlDoc * document ) : mDocument( document ) {}
  virtual ~XmlDocumentManager() {
    xmlFreeDoc( mDocument );
  }

  xmlDoc * get() { return mDocument; }

private:
  xmlDoc * mDocument;
};

} // namespace impl
} // namespace xdmf

#endif // xdmf_impl_NodeList_hpp
