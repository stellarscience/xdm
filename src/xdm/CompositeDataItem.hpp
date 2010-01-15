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
#ifndef xdm_CompositeDataItem_hpp
#define xdm_CompositeDataItem_hpp

#include <xdm/DataItem.hpp>
#include <xdm/ObjectCompositionMixin.hpp>
#include <xdm/RefPtr.hpp>

#include <vector>

#include <xdm/NamespaceMacro.hpp>

XDM_NAMESPACE_BEGIN

/// Class that groups multiple Data nodes together.  CompositeDataItem is a Data
/// type that can hold child Data types.  It is used to construct a large
/// dataset from multiple smaller datasets.
class CompositeDataItem : 
  public DataItem,
  public ObjectCompositionMixin< DataItem > {
public:
  CompositeDataItem();
  
  /// Constructor takes the number of children to hold.
  explicit CompositeDataItem( unsigned int n );

  virtual ~CompositeDataItem();

  XDM_META_ITEM( CompositeDataItem );

  /// Redefinition of visitor traversal from Item.
  virtual void traverse( ItemVisitor& iv );

  /// Write the metadata for a collection of dataitems.
  virtual void writeMetadata( XmlMetadataWrapper& xml );
};

XDM_NAMESPACE_END

#endif // xdm_CompositeDataItem_hpp

