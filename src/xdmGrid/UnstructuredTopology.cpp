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
#include <xdmGrid/UnstructuredTopology.hpp>

#include <sstream>

XDM_GRID_NAMESPACE_BEGIN

UnstructuredTopology::UnstructuredTopology() :
  Topology(),
  mNumberOfElements(),
  mNodesPerElement(),
  mConnectivity() {
}

UnstructuredTopology::~UnstructuredTopology() {
}

void UnstructuredTopology::setNumberOfElements( unsigned int numberOfElements ) {
  mNumberOfElements = numberOfElements;
}

unsigned int UnstructuredTopology::numberOfElements() const {
  return mNumberOfElements;
}

void UnstructuredTopology::setNodesPerElement( unsigned int nodesPerElement ) {
  mNodesPerElement = nodesPerElement;
}

unsigned int UnstructuredTopology::nodesPerElement() const {
  return mNodesPerElement;
}

void UnstructuredTopology::setConnectivity( xdm::DataItem* connectivity ) {
  mConnectivity = connectivity;
}

void UnstructuredTopology::traverse( xdm::ItemVisitor& iv ) {
  if ( mConnectivity.valid() ) {
    mConnectivity->accept( iv );
  }
}

void UnstructuredTopology::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Topology::writeMetadata( xml );

  // Write the number of elements
  xml.setAttribute( "NumberOfElements", mNumberOfElements );

  // Write the nodes per element
  xml.setAttribute( "NodesPerElement", mNodesPerElement );
}

XDM_GRID_NAMESPACE_END

