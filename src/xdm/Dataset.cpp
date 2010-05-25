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
#include <xdm/Dataset.hpp>

namespace xdm {

Dataset::Dataset() :
  mUpdateCallback(),
  mIsInitialized( false ),
  mShape() {
}

Dataset::~Dataset() {
}

RefPtr< BasicDatasetUpdateCallback > Dataset::updateCallback() {
  return mUpdateCallback;
}

RefPtr< const BasicDatasetUpdateCallback > Dataset::updateCallback() const {
  return mUpdateCallback;
}

void Dataset::setUpdateCallback( RefPtr< BasicDatasetUpdateCallback > callback ) {
  mUpdateCallback = callback;
}
  

void Dataset::update( std::size_t seriesIndex ) {
  if ( mUpdateCallback.valid() ) {
    mUpdateCallback->update( this, seriesIndex );
  }
}

bool Dataset::isInitialized() const {
  return mIsInitialized;
}

const DataShape<>& Dataset::shape() const {
  return mShape;
}

DataShape<> Dataset::initialize(
  primitiveType::Value type,
  const DataShape<>& shape,
  const InitializeMode& mode ) {
  mShape = initializeImplementation( type, shape, mode );
  mIsInitialized = true;
  return mShape;
}

void Dataset::serialize(
  const StructuredArray* data,
  const DataSelectionMap& selectionMap )
{
  serializeImplementation( data, selectionMap );
}

void Dataset::deserialize(
  StructuredArray* data,
  const DataSelectionMap& selectionMap )
{
  deserializeImplementation( data, selectionMap );
}

void Dataset::finalize() {
  finalizeImplementation();
  mIsInitialized = false;
  mShape = DataShape<>();
}

} // namespace xdm

