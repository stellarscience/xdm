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
#include <xdm/UniformDataItem.hpp>

#include <xdm/DataSelection.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include <xdm/ThrowMacro.hpp>

XDM_NAMESPACE_BEGIN

namespace {

  struct WriteData {
    Dataset* mDataset;
    WriteData( Dataset* dataset ) : mDataset(dataset) {}
    void operator()( WritableData* writable ) {
      writable->write( mDataset );
    }
  };

} // namespace anon

UniformDataItem::UniformDataItem( 
  primitiveType::Value dataType,
  const DataShape<>& dataspace ) :
  mDataType( dataType ),
  mDataspace( dataspace ),
  mDataset(),
  mWritables() {
}

UniformDataItem::~UniformDataItem() {
}

Dataset* UniformDataItem::dataset() {
  return mDataset.get();
}

const Dataset* UniformDataItem::dataset() const {  
  return mDataset.get();
}

void UniformDataItem::setDataset( Dataset* ds ) {
  mDataset = ds;
}

void UniformDataItem::setDataType( primitiveType::Value dataType ) {
  mDataType = dataType;
}

primitiveType::Value UniformDataItem::dataType() const {
  return mDataType;
}

const DataShape<>& UniformDataItem::dataspace() const {
  return mDataspace;
}

void UniformDataItem::setDataspace( const DataShape<>& dataspace ) {
  typedef DataShape<>::ConstDimensionIterator iterator;
  mDataspace.setRank( 0 );
  for ( iterator it = dataspace.begin(); it != dataspace.end(); ++it ) {
    if ( *it > 0 ) mDataspace.push_back( *it );
  }
}

void UniformDataItem::appendData( WritableData* data ) {
  mWritables.push_back( data );
}

void UniformDataItem::clearData() {
  mWritables.clear();
}

void UniformDataItem::writeMetadata( XmlMetadataWrapper& xml ) {
  DataItem::writeMetadata( xml );

  // write the data item type to "Uniform"
  xml.setAttribute( "ItemType", "Uniform" );
  
  // write the dimensions of the data on disk
  std::stringstream dimensions;
  dimensions << mDataspace;
  xml.setAttribute( "Dimensions", dimensions.str() );

  // write the type of data to the xml metadata.
  switch ( mDataType ) {
  case primitiveType::kChar:
    xml.setAttribute( "NumberType", "Char" );
    xml.setAttribute( "Precision", sizeof( char ) );
    break;
  case primitiveType::kUnsignedChar:
    xml.setAttribute( "NumberType", "UChar" );
    xml.setAttribute( "Precision", sizeof( unsigned char ) );
    break;
  case primitiveType::kInt:
    xml.setAttribute( "NumberType", "Int" );
    xml.setAttribute( "Precision", sizeof( int ) );
    break;
  case primitiveType::kUnsignedInt:
    xml.setAttribute( "NumberType", "UInt" );
    xml.setAttribute( "Precision", sizeof( unsigned int ) );
    break;
  case primitiveType::kFloat:
    xml.setAttribute( "NumberType", "Float" );
    xml.setAttribute( "Precision", sizeof( float ) );
    break;
  case primitiveType::kDouble:
    xml.setAttribute( "NumberType", "Float" );
    xml.setAttribute( "Precision", sizeof( double ) );
    break;
  default:
    XDM_THROW( std::invalid_argument( "Unrecognized dataset number type" ) );
    break;
  }

  // write the format of the dataset on disk.
  xml.setAttribute( "Format", mDataset->format() );

  // Allow the dataset to configure the text content for this item.
  mDataset->writeTextContent( xml );
}

void UniformDataItem::serializeData() {
  mDataset->initialize( mDataType, mDataspace );
  std::for_each( mWritables.begin(), mWritables.end(),
    WriteData( mDataset.get() ) );
  mDataset->finalize();
}

XDM_NAMESPACE_END

