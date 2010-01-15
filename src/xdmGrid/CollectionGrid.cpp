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
#include <xdmGrid/CollectionGrid.hpp>

#include <algorithm>
#include <stdexcept>

#include <xdm/ThrowMacro.hpp>

XDM_GRID_NAMESPACE_BEGIN

CollectionGrid::CollectionGrid( CollectionType type ) :
  Grid(),
  mType( type ) {
}

CollectionGrid::~CollectionGrid() {
}

void CollectionGrid::setType( CollectionType t ) {
  mType = t;
}

CollectionGrid::CollectionType CollectionGrid::type() const {
  return mType;
}

void CollectionGrid::traverse( xdm::ItemVisitor& iv ) {
  Grid::traverse( iv );
  std::for_each(
    xdm::begin< Grid >( *this ),
    xdm::end< Grid >( *this ),
    xdm::ApplyVisitor( iv ) );
}

void CollectionGrid::writeMetadata( xdm::XmlMetadataWrapper& xml ) {
  Grid::writeMetadata( xml );

  // write the grid type as a collection
  xml.setAttribute( "GridType", "Collection" );

  // write the type of collection.
  switch ( mType ) {
  case kSpatial:
    xml.setAttribute( "CollectionType", "Spatial" );
    break;
  case kTemporal:
    xml.setAttribute( "CollectionType", "Temporal" );
    break;
  default:
    XDM_THROW( std::invalid_argument( "Unrecognized Grid type" ) );
    break;
  }
}

XDM_GRID_NAMESPACE_END

