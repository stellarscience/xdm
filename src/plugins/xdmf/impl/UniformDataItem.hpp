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
#ifndef xdmf_impl_UniformDataItem_hpp
#define xdmf_impl_UniformDataItem_hpp

#include <xdmf/impl/Input.hpp>

#include <xdm/UniformDataItem.hpp>

namespace xdmf {
namespace impl {

/// XDMF implementation of UniformDataItem to handle time stepping.
class UniformDataItem :
  public xdm::UniformDataItem,
  public Input {
public:
  UniformDataItem(
    xdm::RefPtr< XmlDocumentManager > doc,
    xdm::RefPtr< SharedNodeVector > stepNodes,
    const std::string& xpathExpr );
  virtual ~UniformDataItem();

  virtual void read( xmlNode * node, TreeBuilder& builder );

protected:
  virtual void updateState( std::size_t seriesIndex );

};

} // namespace impl
} // namespace xdmf

#endif // xdmf_impl_UniformDataItem_hpp
