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
#include <xdmHdf/FileIdentifierRegistry.hpp>

#include <hdf5.h>

XDM_HDF_NAMESPACE_BEGIN

xdm::RefPtr< FileIdentifierRegistry > FileIdentifierRegistry::sInstance;

FileIdentifierRegistry* FileIdentifierRegistry::instance() {
  if ( ! sInstance.valid() ) {
    sInstance = new FileIdentifierRegistry;
  }
  return sInstance;
}

FileIdentifierRegistry::FileIdentifierRegistry() :
  mIdentifierMapping() {
}

xdm::RefPtr< FileIdentifier > FileIdentifierRegistry::findOrCreateIdentifier(
  const std::string& key ) {
  // try to find an existing identifier in the map
  IdentifierMapping::iterator it = mIdentifierMapping.find( key );
  if ( it != mIdentifierMapping.end() ) {
    return it->second;
  }

  // no such file opened yet, we create it here
  hid_t fileId = H5Fcreate( 
    key.c_str(), 
    H5F_ACC_TRUNC,
    H5P_DEFAULT,
    H5P_DEFAULT );

  xdm::RefPtr< FileIdentifier > result( new FileIdentifier( fileId ) );
  mIdentifierMapping[key] = result;
  return result;
}

XDM_HDF_NAMESPACE_END

