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
#include <xdm/CompositeDataItem.hpp>

#include <algorithm>

#include <cassert>

XDM_NAMESPACE_BEGIN

CompositeDataItem::CompositeDataItem() :
  DataItem(),
  ObjectCompositionMixin< DataItem >() {
}

CompositeDataItem::CompositeDataItem( unsigned int n ) :
  DataItem(),
  ObjectCompositionMixin< DataItem >( n ) {
}

CompositeDataItem::~CompositeDataItem() {
}

void CompositeDataItem::traverse( ItemVisitor& iv ) {
  std::for_each( begin(), end(), ApplyVisitor( iv ) );
}

void CompositeDataItem::writeMetadata( XmlMetadataWrapper& xml ) {
  DataItem::writeMetadata( xml );

  // write my ItemType as a collection.
  xml.setAttribute( "ItemType", "Tree" );
}

XDM_NAMESPACE_END

