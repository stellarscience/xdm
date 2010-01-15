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
#include <xdmGrid/RectilinearMesh.hpp>

#include <stdexcept>

#include <xdm/ThrowMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

RectilinearMesh::RectilinearMesh() :
  StructuredTopology() {
}

RectilinearMesh::~RectilinearMesh() {
}

void RectilinearMesh::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  StructuredTopology::writeMetadata( xml );

  unsigned int rank = shape().rank();
  switch ( rank ) {
  case 2: 
    xml.setAttribute( "TopologyType", "2DRectMesh" );
    break;
  case 3:
    xml.setAttribute( "TopologyType", "3DRectMesh" );
    break;
  default:
    {
      std::stringstream msg;
      msg << "Unsupported number of dimensions for rectinlinear mesh: ";
      msg << rank;
      XDM_THROW( std::domain_error( msg.str() ) );
    }
    break;
  }
}


XDM_GRID_NAMESPACE_END

