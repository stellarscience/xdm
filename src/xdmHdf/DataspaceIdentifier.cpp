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
#include <xdmHdf/DataspaceIdentifier.hpp>

XDM_HDF_NAMESPACE_BEGIN

xdm::RefPtr< DataspaceIdentifier > createDataspaceIdentifier(
  const xdm::DataShape<>& shape ) {
  xdm::DataShape< hsize_t > hdfShape ( shape );
  hid_t identifier = H5Screate_simple( 
    hdfShape.rank(), 
    &hdfShape[0],
    NULL );
  return xdm::RefPtr< DataspaceIdentifier >( 
    new DataspaceIdentifier( identifier ) );
}

XDM_HDF_NAMESPACE_END

