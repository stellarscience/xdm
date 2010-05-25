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
#include <xdm/DataSelectionMap.hpp>

#include <xdm/AllDataSelection.hpp>
#include <xdm/DataSelection.hpp>

namespace xdm {

DataSelectionMap::DataSelectionMap() :
  mDomain( new AllDataSelection ),
  mRange( new AllDataSelection ) {
}

DataSelectionMap::DataSelectionMap( 
  RefPtr< DataSelection > domain,
  RefPtr< DataSelection > range ) :
  mDomain( domain ),
  mRange( range ) {
}

DataSelectionMap::~DataSelectionMap() {
}

RefPtr< const DataSelection > DataSelectionMap::domain() const {
  return mDomain;
}

void DataSelectionMap::setDomain( RefPtr< DataSelection > domain ) {
  mDomain = domain;
}

RefPtr< const DataSelection > DataSelectionMap::range() const {
  return mRange;
}

void DataSelectionMap::setRange( RefPtr< DataSelection > range ) {
  mRange = range;
}

} // namespace xdm

