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
#include <xdmHdf/GroupIdentifier.hpp>

XDM_HDF_NAMESPACE_BEGIN

xdm::RefPtr< GroupIdentifier > createGroupIdentifier(
  hid_t parent, const std::string& name ) {
  // check if the group already exists within the parent
  htri_t exists = H5Lexists( parent, name.c_str(), H5P_DEFAULT );
  if ( (exists>=0) && exists ) {
    hid_t groupId = H5Gopen( parent, name.c_str(), H5P_DEFAULT );
    return xdm::RefPtr< GroupIdentifier >( new GroupIdentifier( groupId ) );
  }

  // it doesn't already exist, create it
  hid_t groupId = H5Gcreate( 
    parent, 
    name.c_str(), 
    H5P_DEFAULT, 
    H5P_DEFAULT,
    H5P_DEFAULT );
  return xdm::RefPtr< GroupIdentifier >( new GroupIdentifier( groupId ) );
}

XDM_HDF_NAMESPACE_END

