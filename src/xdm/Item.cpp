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
#include <xdm/Item.hpp>

#include <xdm/ItemVisitor.hpp>

namespace xdm {

Item::Item() {
}

Item::~Item() {
}

void Item::setName( const std::string& name ) {
  mName = name;
}

const std::string& Item::name() const {
  return mName;
}

const char * Item::className() const {
  return "Item";
}

void Item::accept( ItemVisitor& iv ) {
  iv.apply( *this );
}

void Item::traverse( ItemVisitor& ) {
  // No-op
}

RefPtr< BasicItemUpdateCallback > Item::updateCallback() {
  return mUpdateCallback;
}

RefPtr< const BasicItemUpdateCallback > Item::updateCallback() const {
  return mUpdateCallback;
}

void Item::setUpdateCallback( RefPtr< BasicItemUpdateCallback > callback ) {
  mUpdateCallback = callback;
}

void Item::updateState( std::size_t seriesIndex ) {
  // do nothing
}

void Item::writeMetadata( XmlMetadataWrapper& xml ) {
  xml.setTag( className() );
  if ( !mName.empty() ) {
    xml.setAttribute( "Name", mName );
  }
}

} // namespace xdm

