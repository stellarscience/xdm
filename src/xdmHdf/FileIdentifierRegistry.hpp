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
#ifndef xdmHdf_FileIdentifierRegistry_hpp
#define xdmHdf_FileIdentifierRegistry_hpp

#include <xdmHdf/FileIdentifier.hpp>

#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>

#include <map>
#include <string>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

/// Singleton registry to hold references to an HDF file identifier.  According
/// to the HDF documentation, a single application should open a file only once.
/// This registry allows that to happen by caching the identifier for all open
/// files indexed by filename.
class FileIdentifierRegistry : public xdm::ReferencedObject {
public:
  static FileIdentifierRegistry* instance();
  
  /// Get or create an identifier for a given file name.
  xdm::RefPtr< FileIdentifier > findOrCreateIdentifier( 
    const std::string& key );

private:
  FileIdentifierRegistry();

  static xdm::RefPtr< FileIdentifierRegistry > sInstance;
  typedef std::map< std::string, xdm::RefPtr< FileIdentifier > >
    IdentifierMapping;
  IdentifierMapping mIdentifierMapping;
};

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_FileIdentifierRegistry_hpp

