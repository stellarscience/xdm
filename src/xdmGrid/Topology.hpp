//==============================================================================
// This software developed by Stellar Science Ltd Co and the U.S. Government.  
// Copyright (C) 2007 Stellar Science. Government-purpose rights granted.      
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
#ifndef xdmGrid_Topology_hpp
#define xdmGrid_Topology_hpp

#include <xdm/DataItem.hpp>
#include <xdm/Item.hpp>
#include <xdm/DataShape.hpp>
#include <xdm/ObjectCompositionMixin.hpp>

#include <xdmGrid/NamespaceMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

/// Base class for topological descriptions of a grid.  The topological
/// properties of a grid are those which are invariant under rotations,
/// translations, and scale.  Connectivity is the most important topological
/// property.
///
/// A Grid consists of elements (or cells) and nodes.
class Topology : 
  public xdm::Item,
  public xdm::ObjectCompositionMixin< xdm::DataItem > {
public:
  Topology();
  virtual ~Topology();

  XDM_META_ITEM( Topology );

  virtual void traverse( xdm::ItemVisitor& iv );

  virtual void writeMetadata( xdm::XmlMetadataWrapper& xml );
};

XDM_GRID_NAMESPACE_END

#endif // xdmGrid_Topology_hpp

