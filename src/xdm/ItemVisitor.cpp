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
#include <xdm/ItemVisitor.hpp>

#include <xdm/CompositeDataItem.hpp>
#include <xdm/DataItem.hpp>
#include <xdm/UniformDataItem.hpp>

#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

ItemVisitor::ItemVisitor() {
}

ItemVisitor::~ItemVisitor() {
}

void ItemVisitor::apply( Item& item ) {
  traverse( item );
}

void ItemVisitor::apply( DataItem& item ) {
  apply( static_cast< Item& >( item ) );
}

void ItemVisitor::apply( CompositeDataItem& item ) {
  apply( static_cast< DataItem& >( item ) );
}

void ItemVisitor::apply( UniformDataItem& item ) {
  apply( static_cast< DataItem& >( item ) );
}

void ItemVisitor::captureState( BinaryOStream& ) {
  // no-op
}

void ItemVisitor::restoreState( BinaryIStream& ) {
  // no-op
}

void ItemVisitor::reset() {
  // no-op
}

XDM_NAMESPACE_END

