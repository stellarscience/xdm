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
#include <xdm/DataSelection.hpp>
#include <xdm/UniformDataItem.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include <xdm/ThrowMacro.hpp>

namespace xdm {

namespace {

}

UniformDataItem::UniformDataItem() :
  mDataType( primitiveType::kFloat ),
  mDataspace(),
  mDataset(),
  mData() {
}

UniformDataItem::UniformDataItem(
  primitiveType::Value dataType,
  const DataShape<>& dataspace ) :
  mDataType( dataType ),
  mDataspace( dataspace ),
  mDataset(),
  mData() {
}

UniformDataItem::~UniformDataItem() {
}

RefPtr< Dataset > UniformDataItem::dataset() {
  return mDataset;
}

RefPtr< const Dataset > UniformDataItem::dataset() const {
  return mDataset;
}

void UniformDataItem::setDataset( RefPtr< Dataset > ds ) {
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

void UniformDataItem::setData( RefPtr< MemoryAdapter > data ) {
  mData = data;
}

RefPtr< MemoryAdapter > UniformDataItem::data() {
  return mData;
}

RefPtr< const MemoryAdapter > UniformDataItem::data() const {
  return mData;
}

void UniformDataItem::clearData() {
  mData.reset();
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
  case primitiveType::kLongUnsignedInt:
    xml.setAttribute( "NumberType", "UInt" );
    xml.setAttribute( "Precision", sizeof( long unsigned int ) );
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

void UniformDataItem::initializeDataset( const Dataset::InitializeMode& mode ) {
  mDataspace = mDataset->initialize( mDataType, mDataspace, mode );
}

void UniformDataItem::serializeData() {
  if ( !mData ) {
    XDM_THROW( DataAccessError() );
  }
  mData->write( mDataset.get() );
}

void UniformDataItem::deserializeData() {
  if ( !mData ) {
    XDM_THROW( DataAccessError() );
  }
  mData->read( mDataset.get() );
}

void UniformDataItem::finalizeDataset() {
  mDataset->finalize();
}

bool UniformDataItem::serializationRequired() const {
  return mData->requiresWrite();
}

bool UniformDataItem::validateBounds( const xdm::DataShape<>& shape ) const {
  if ( shape.rank() != mDataspace.rank() ) return false;
  for ( xdm::DataShape<>::size_type i = 0; i < mDataspace.rank(); ++i ) {
    if ( shape[i] >= mDataspace[i] ) return false;
  }
  return true;
}

RefPtr< const StructuredArray > UniformDataItem::array() const {
  if ( !mData ) {
    XDM_THROW( DataAccessError() );
  }
  if ( !mData->isMemoryResident() && mData->requiresWrite() ) {
    UniformDataItem* mutableThis = const_cast< UniformDataItem* >(this);
    mutableThis->initializeDataset( Dataset::kRead );
    mutableThis->deserializeData();
    mutableThis->finalizeDataset();
  }
  return mData->array();
}

RefPtr< StructuredArray > UniformDataItem::array() {
  return xdm::const_pointer_cast< StructuredArray >(
    static_cast< const UniformDataItem& >(*this).array()
  );
}

} // namespace xdm

