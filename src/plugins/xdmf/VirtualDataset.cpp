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
#include <xdmf/VirtualDataset.hpp>

#include <xdmf/XdmfHelpers.hpp>

#include <xdm/CollectMetadataOperation.hpp>
#include <xdm/SerializeDataOperation.hpp>
#include <xdm/ReferencedObject.hpp>
#include <xdm/RefPtr.hpp>
#include <xdm/UpdateVisitor.hpp>
#include <xdm/XmlObject.hpp>
#include <xdm/XmlOutputStream.hpp>

#include <xdmGrid/Domain.hpp>
#include <xdmGrid/CollectionGrid.hpp>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

XDMF_NAMESPACE_BEGIN

VirtualDataset::VirtualDataset( 
  const std::string& metadataBaseName,
  xdm::Dataset::InitializeMode mode ) :
  TimeSeries( mode ),
  mBaseName( metadataBaseName ),
  mTimeStep( 0 )
{
}

VirtualDataset::~VirtualDataset()
{
}

void VirtualDataset::open() 
{
  mTimeStep = 0;
}

void VirtualDataset::updateGrid( xdm::RefPtr< xdmGrid::Grid > grid ) {
  // update the grid for the new timestep.
  xdm::UpdateVisitor update;
  grid->accept( update );
}

void VirtualDataset::writeGridMetadata( xdm::RefPtr< xdmGrid::Grid > grid ) {
  // add the grid to a new domain for a valid layout
  xdm::RefPtr< xdmGrid::Domain > domain( new xdmGrid::Domain );
  domain->addGrid( grid );
  
  // open a new xml stream to write this timestep to
  std::stringstream outputName;
  outputName << mBaseName << ".";
  outputName << std::setfill('0') << std::setw( 7 ) << mTimeStep;
  outputName << ".xmf";
  std::fstream ostr( outputName.str().c_str(), std::ios::out );
  xdm::XmlOutputStream xml( ostr );

  // write the timestep to the xml stream
  xdm::CollectMetadataOperation collect;
  domain->accept( collect );
  
  xdm::RefPtr< xdm::XmlObject > xdmf = xdmf::createXdmfRoot();
  xdmf->appendChild( collect.result() );
  
  xml.writeObject( xdmf );

  xml.closeStream();

}

void VirtualDataset::writeGridData( xdm::RefPtr< xdmGrid::Grid > grid )
{
  // serialize the heavy data
  xdm::SerializeDataOperation serializer( mode() );
  grid->accept( serializer ); 
  mTimeStep++;
}

void VirtualDataset::close()
{
}

XDMF_NAMESPACE_END

