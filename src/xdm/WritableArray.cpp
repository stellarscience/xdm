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
#include <xdm/WritableArray.hpp>

#include <xdm/DataSelection.hpp>
#include <xdm/Dataset.hpp>
#include <xdm/StructuredArray.hpp>

XDM_NAMESPACE_BEGIN

WritableArray::WritableArray( StructuredArray* array, bool isDynamic ) :
  WritableData( isDynamic ),
  mArray( array ),
  mSelectionMap()
{
}

WritableArray::~WritableArray()
{
}

StructuredArray* WritableArray::array() {
  return mArray.get();
}

const StructuredArray* WritableArray::array() const {
  return mArray.get();
}

void WritableArray::setArray( StructuredArray* array ) {
  mArray = array;
}

const DataSelectionMap& WritableArray::selectionMap() const {
  return mSelectionMap;
}

void WritableArray::setSelectionMap( const DataSelectionMap& selectionMap ) {
  mSelectionMap = selectionMap;
}

void WritableArray::writeImplementation( Dataset* dataset ) {
  dataset->serialize( mArray, mSelectionMap );
}

XDM_NAMESPACE_END

