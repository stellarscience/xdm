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
#include <xdmHdf/FileIdentifierRegistry.hpp>

#include <xdm/ThrowMacro.hpp>

#include <hdf5.h>

#include <stdexcept>

#include <sys/stat.h>

namespace xdmHdf {

xdm::RefPtr< FileIdentifierRegistry > FileIdentifierRegistry::sInstance;

xdm::RefPtr< FileIdentifierRegistry > FileIdentifierRegistry::instance() {
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

  // file not yet opened, check if it exists on disk.
  struct stat buf;
  hid_t fileId;
  if ( stat( key.c_str(), &buf ) == 0 ) {
    // file exists open it
    fileId = H5Fopen(
      key.c_str(),
      H5F_ACC_RDWR,
      H5P_DEFAULT );
  } else {
    // file does not exist, create it
    fileId = H5Fcreate( 
      key.c_str(), 
      H5F_ACC_TRUNC,
      H5P_DEFAULT,
      H5P_DEFAULT );
  }

  // if the identifier is still bad, then something is wrong
  if ( fileId < 0 ) {
    XDM_THROW( std::runtime_error( "Failed to create file" ) );
  }

  // we have a good identifier, return the reference counted resource. 
  xdm::RefPtr< FileIdentifier > result( new FileIdentifier( fileId ) );
  mIdentifierMapping[key] = result;
  return result;
}

void FileIdentifierRegistry::closeAllIdentifiers() {
  mIdentifierMapping.clear();
}

} // namespace xdmHdf

