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
#ifndef xdmHdf_GroupIdentifier_hpp
#define xdmHdf_GroupIdentifier_hpp

#include <xdmHdf/ResourceIdentifier.hpp>

#include <xdm/RefPtr.hpp>

#include <hdf5.h>

#include <string>

#include <xdmHdf/NamespaceMacro.hpp>

XDM_HDF_NAMESPACE_BEGIN

class GroupReleaseFunctor {
public:
  herr_t operator()( hid_t identifier ) {
    return H5Gclose( identifier );
  }
};

typedef ResourceIdentifier< GroupReleaseFunctor > GroupIdentifier;

xdm::RefPtr< GroupIdentifier > createGroupIdentifier( 
  hid_t parent, const std::string& name );

XDM_HDF_NAMESPACE_END

#endif // xdmHdf_GroupIdentifier_hpp

