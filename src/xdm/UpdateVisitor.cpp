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
#include <xdm/UpdateVisitor.hpp>

#include <xdm/Dataset.hpp>
#include <xdm/UniformDataItem.hpp>

namespace xdm {

UpdateVisitor::UpdateVisitor( std::size_t seriesIndex ) :
  mSeriesIndex( seriesIndex ) {
}

UpdateVisitor::~UpdateVisitor() {
}

void UpdateVisitor::apply( Item& item ) {
  // Tell the Item to update its internal state.
  item.updateState( mSeriesIndex );

  // Call an application defined callback if it exists.
  if ( RefPtr< BasicItemUpdateCallback > callback = item.updateCallback() ) {
    callback->update( item, mSeriesIndex );
  }

  // Continue with this Item's
  traverse( item );
}

void UpdateVisitor::apply( UniformDataItem& item ) {
  // Call the Item's own update callback.
  apply( static_cast< Item& >( item ) );

  // If the Item has been assigned a dataset, call its callback too.
  RefPtr< Dataset > itemDataset = item.dataset();
  if ( itemDataset ) {
    itemDataset->update( mSeriesIndex );
  }

  traverse( item );
}

void updateToIndex( Item& item, std::size_t seriesIndex ) {
  UpdateVisitor v( seriesIndex );
  item.accept( v );
}

} // namespace xdm

