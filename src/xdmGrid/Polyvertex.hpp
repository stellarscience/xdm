//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.
// Copyright (C) 2009-2010 Stellar Science. Government-purpose rights granted.
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
#ifndef xdmGrid_Polyvertex_hpp
#define xdmGrid_Polyvertex_hpp

#include <xdmGrid/UnstructuredTopology.hpp>



namespace xdmGrid {

class Polyvertex : public xdmGrid::UnstructuredTopology {
public:
  Polyvertex();
  virtual ~Polyvertex();

  XDM_META_ITEM( "Polyvertex" );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );
private:
  size_t mNumberOfPoints;

  // Cannot change the type from Polyvertex.
  void setElementTopology( xdm::RefPtr< const ElementTopology > topo );
};

} // namespace xdmGrid

#endif // xdmGrid_Polyvertex_hpp
